#include "NPC.h"
#include <iostream>
#include <PARKEngine/PARKEngine.h>
#include "Matrix/Node.h"
#include "Matrix/Matrix.h"
#include "Edificio.h"
#include <limits>
#include <cmath>

//TODO: rotar los NPC en función de la dirección a la que van
NPC::NPC() :pq(0), hasPath(false), isInBuilding_(false), node_(nullptr), prevNode_(nullptr), nextNode_(nullptr), isInQueue_(false), lastAttraction_("N/A"), isLeaving_(false)
{
}

NPC::~NPC()
{
	std::cout << "Destructora de NPC" << std::endl;
}
void NPC::start()
{
	//Gets matrix
	matrix_ = SceneManager::instance()->currentState()->getEntitiesWithComponent<Matrix>()[0]->getComponent<Matrix>();

	//Get initial node
	Node* initialNode = matrix_->getEntrance();
	node_ = prevNode_ = initialNode;
	//Set position to it (coming outside the park)
	Vector3 pos = initialNode->getBrotherComponent<Transform>()->getPosition();
	getBrotherComponent<Transform>()->setPosition(pos + Vector3(0, 10, 100));
}

std::string NPC::getDetailedInfo()
{
	std::string s = entity_->getName() + "\n" + fun_.name_ + ": " + std::to_string(getFun().value_) + "\n" +
		hunger_.name_ + ": " + std::to_string(getHunger().value_) + "\n " + peepee_.name_ + ": " + std::to_string(getPeepee().value_) + "\n";
	return s;
}


void NPC::update(unsigned int time)
{
	//Not currently inside a building
	if (!isInBuilding_)
	{
		//Update stats
		float delta = ((float)time / 1000);
		fun_.consume(delta, true);
		hunger_.consume(delta, true);
		peepee_.consume(delta, true);

		//1. NPC has an avaiable path
		if (hasPath)
			followPath(time);
		else if (isInQueue_) {}
		//2. NPC realises he needs to eat/pee/he's bored
		else if (lowStats())
		{
			lookForBuildings();
			//He can't reach the building he needs (get out of the park)
			//And its not in a queue (This prevents npc in queues to leave the park and with so crash the game)
			//And not in a building
			if (!hasPath && !isInQueue_ && !isInBuilding_)
				exitPARK();
		}

		//3. He just walks around
		else
			deambulate(time);
	}
	else
	{
		//Needs restoring
		peepee_.restore((float)actualBuilding_->getPeePeeValue() / (float)actualBuilding_->getDuration() * time);
		fun_.restore((float)actualBuilding_->getFunValue() / (float)actualBuilding_->getDuration() * time);
		hunger_.restore((float)actualBuilding_->getHungryValue() / (float)actualBuilding_->getDuration() * time);
	}
}

void NPC::followPath(unsigned int time)
{
	//We can be in a path when...
	//1. We're going to a certain building
	//2. We're getting out of the park
	Node* n = movements.top();
	//Not in that node yet (move towards it)
	if (node_ != n)
		moveToNode(n, time);
	//Arrived to the node
	else
	{
		//Pop movement
		movements.pop();
		//If path ended
		if (movements.empty())
		{
			hasPath = false;
			//We arrived to the building
			if (actualBuilding_ != nullptr)
				enterQueue(); //Tries to enter
			//We arrived to the PARK entrance
			else //TODO: esto se deja basura seguro
			{
				getBrotherComponent<MeshRenderer>()->setVisible(false);
				entity_->setActive(false);
				SceneManager::instance()->currentState()->removeEntity(getEntity()->getName()); //remove from scene
			}
				
		}
	}
}

void NPC::enterQueue()
{
	//Hay hueco en la cola
	if (actualBuilding_->encolar(getEntity()))
	{
		isInQueue_ = true;
		speed_ /= 1.5;
	}
}

void NPC::enterAttraction()
{
	isInBuilding_ = true;
	isInQueue_ = false;
	lastAttraction_ = actualBuilding_->getEntity()->getName();
}

Edificio* NPC::getBuilding(Node* eNode)
{
	//All buildings in scene
	std::vector<Entity*> builds = SceneManager::instance()->currentState()->getEntitiesWithComponent<Edificio>();
	std::vector<Entity*>::iterator it = builds.begin();
	Edificio* building = nullptr;

	//Find the building to enter
	bool found = false;
	while (it != builds.end() && !found)
	{
		building = (*it)->getComponent<Edificio>();
		if (building != nullptr && building->getEntryNode() == eNode)
			found = true;
		it++;
	}

	return building;
}

void NPC::getOutofAttraction()
{
	//Get out of the building and set position
	node_ = prevNode_ = actualBuilding_->getExitNode();
	nextNode_ = nullptr;
	Vector3 pos = node_->getBrotherComponent<Transform>()->getPosition();
	getBrotherComponent<Transform>()->setPosition(pos + Vector3(0, 10, 0));

	//Flags
	actualBuilding_ = nullptr;
	isInBuilding_ = false;
}

void NPC::exitPARK()
{
	std::cout << "NPC::ExitPark() Invocado" << std::endl;
	hasPath = true;
	isLeaving_ = true;
	movements.push(matrix_->getEntrance());

	speed_ *= 1.5;
	
}

void NPC::setNode(Node * node)
{
	node_ = node;
}

Node * NPC::getNode()
{
	return node_;
}

void NPC::load(json file)
{
	speed_ = file["speed"];
	exigency_ = file["exigency"];

	json stat = file["stats"];
	fun_ = Stat(stat[0]["name"], stat[0]["value"], stat[0]["maxValue"], exigency_, stat[0]["decreases"]);
	hunger_ = Stat(stat[1]["name"], stat[1]["value"], stat[1]["maxValue"], exigency_, stat[1]["decreases"]);
	peepee_ = Stat(stat[2]["name"], stat[2]["value"], stat[2]["maxValue"], exigency_, stat[2]["decreases"]);

	//Nombre aleatorio del archivo
	json names =  ResourceManager::instance()->getJsonByKey(file["nameFile"]);
	int rnd = std::rand() % names.size();
	entity_->setName(names[rnd]);
}

void NPC::lookForBuildings()
{
	//Pillamos variables
	int N = matrix_->getSize(0) * matrix_->getSize(1);
	pq = IndexPQ<int>(N);
	distTo = std::vector<int>(N);
	nodeTo = std::vector<int>(N);

	//Inicializamos las distancias a infinito
	for (int v = 0; v < N; v++)
		distTo[v] = numeric_limits<int>::max();


	//Metemos el nodo actual con prioridad 0
	Vector2 nodePos = node_->getMatrixPos();
	int nodeIndex = calculateIndex(nodePos.x, nodePos.y);
	pq.push(nodeIndex, 0);
	distTo[nodeIndex] = 0; //Distancia 0
	nodeTo[nodeIndex] = nodeIndex;

	bool atraccion = false;
	Node* nodoActual = nullptr;
	//Vamos metiendo y sacando elementos de la cola de prioridades variables
	while (!pq.empty() && !atraccion)
	{
		IndexPQ<int>::Par n = pq.top();
		nodoActual = matrix_->getEntityNode(n.elem)->getComponent<Node>();
		pq.pop();
		std::list<Entity*> ady = matrix_->getAdj(nodoActual->getEntity(), 1, 1); //radio de 1
																				 //Adyacentes al nodo actual
		for (Entity* e : ady)
		{
			Node* adyNode = e->getComponent<Node>();
			Vector2 srcPos = nodoActual->getMatrixPos();
			Vector2 adyPos = adyNode->getMatrixPos();
			//Quitamos diagonales y la propia casilla
			if (adyacenteCorrecta(srcPos, adyPos))
			{
				if (adyNode->getType() == Node::NodeType::EntryRoad)
				{
					relax(n.elem, calculateIndex(adyPos.x, adyPos.y));
					Edificio* b = getBuilding(adyNode);

					//The building provides us what we need
					if (b->getType() == lowerStat().restorer_)
					{
						nodoActual = adyNode;
						actualBuilding_ = b;
						atraccion = true;
						break;
					}
				}
				//Explore other nodes
				else if (adyNode->getType() == Node::NodeType::Road || adyNode->getType() == Node::NodeType::ExitRoad)
					relax(n.elem, calculateIndex(adyPos.x, adyPos.y));
			}
		}
	}

	//Rellenamos la cola de movimientos
	if (atraccion)
	{
		int index = calculateIndex(nodoActual->getMatrixPos().x, nodoActual->getMatrixPos().y);
		do
		{
			//Get node 
			Node* n = matrix_->getEntityNode(index)->getComponent<Node>();
			//Put it in the element list
			movements.push(n);
			//Previous index
			index = nodeTo[index];
		} while (nodeTo[index] != index);
		hasPath = true;
		speed_ *= 1.5;
	}
	else
		std::cout << "Can't restore: " << lowerStat().name_ << ". I'm leaving PARK" << std::endl;
}


void NPC::deambulate(unsigned int time)
{
	//Where to go?
	if (nextNode_ == nullptr)
	{
		//1. FILTRAMOS LAS CASILLAS ADYACENTES
		std::list<Entity*> ady = matrix_->getAdj(node_->getEntity(), 1, 1); //radio de 1
		std::list<Entity*>::iterator it = ady.begin();
		std::vector<Node*> candidates;
		while (it != ady.end())
		{
			Node* adyNode = (*it)->getComponent<Node>();
			Vector2 srcPos = node_->getMatrixPos();
			Vector2 adyPos = adyNode->getMatrixPos();
			//1. Adyacente correcta   2. Es un camino  3. No venimos de ahí
			if (adyacenteCorrecta(srcPos, adyPos) &&
				(adyNode->getType() == Node::NodeType::Road || adyNode->getType() == Node::NodeType::EntryRoad || adyNode->getType() == Node::NodeType::ExitRoad) &&
				adyNode != prevNode_)
				candidates.push_back(adyNode);
			it++;
		}


		//2. ELEGIR UN NODO
		//Solo hay una dirección (volvemos)
		if (candidates.size() == 0)
		{
			if (prevNode_ != node_)
				nextNode_ = prevNode_;
		}
		else
		{
			int index = std::rand() % candidates.size();
			nextNode_ = candidates.at(index);
		}
	}
	else
		moveToNode(nextNode_, time);
}

void NPC::relax(int srcIndex, int destIndex)
{
	if (distTo[destIndex] > distTo[srcIndex] + 1)
	{
		//Añadimos el tiempo de espera de la página en si
		distTo[destIndex] = distTo[srcIndex] + 1;
		nodeTo[destIndex] = srcIndex;
		//En la función update se contempla que la clave no estuviera ya
		pq.update(destIndex, distTo[destIndex]);
	}
}

int NPC::calculateIndex(int i, int j)
{
	return i * matrix_->getSize(1) + j;
}

bool NPC::adyacenteCorrecta(Vector2 src, Vector2 dst)
{
	//Es una goddamn puerta XOR
	return ((src.x == dst.x || src.y == dst.y) && (src.x != dst.x || src.y != dst.y));
}

bool NPC::isInNode(Node* n)
{
	Vector3 nodeSize = matrix_->getNodeSize();
	Vector3 newNodeMin = n->getBrotherComponent<Transform>()->getPosition() - nodeSize / 3;
	Vector3 newNodeMax = n->getBrotherComponent<Transform>()->getPosition() + nodeSize / 3;

	//Check AABB
	Vector3 actualPos = getBrotherComponent<Transform>()->getPosition();
	if (actualPos.x > newNodeMin.x && actualPos.x < newNodeMax.x && actualPos.z > newNodeMin.z && actualPos.z < newNodeMax.z)
		return true;
	return false;
}
void NPC::moveToNode(Node* n, int deltaTime)
{
	Transform* trans = getBrotherComponent<Transform>();
	Transform* nextTrans = n->getBrotherComponent<Transform>();

	//Posición destino
	//TODO: movimiento menos lineal
	//float random = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX); //random de 0 - 1
	//Vector3 nodeSize = matrix_->getNodeSize();
	//Vector3 dstPos = (nextTrans->getPosition() - nodeSize / 2) + Vector3(nodeSize.x * random, 0, nodeSize.z * random);
	//Vector unitario de la dirección
	Vector3 delta = (nextTrans->getPosition() - trans->getPosition());
	delta.normalise();
	//Lo multiplicamos por el deltaTime y la velocidad del NPC
	delta = delta * speed_ * ((float)deltaTime / 10);
	delta.y = 0;//We dont want to move in that axis

	//Lo movemos
	trans->translate(delta);

	//Update current node
	if (isInNode(n))
	{
		prevNode_ = node_;
		nextNode_ = nullptr;
		setNode(n);
	}
}

bool NPC::lowStats()
{
	return (fun_.value_ < (fun_.MAX_VALUE / 10) * 3 || peepee_.value_ >(peepee_.MAX_VALUE / 10) * 7 || hunger_.value_ > (hunger_.MAX_VALUE / 10) * 7);
}

const Stat& NPC::lowerStat()
{
	float funPriority = fun_.value_;
	float hungerPriority = hunger_.MAX_VALUE - hunger_.value_;
	float peepeePriority = peepee_.MAX_VALUE - peepee_.value_;

	if (funPriority < peepeePriority && funPriority < hungerPriority)
		return fun_;
	else if (peepeePriority < funPriority && peepeePriority < hungerPriority)
		return peepee_;
	else
		return hunger_;
}
