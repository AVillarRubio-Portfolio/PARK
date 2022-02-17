#include "GameState.h"
#include "PARKManagers.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "Component.h"
#include "Button.h"
#include "OgreIncludes.h"

GameState::GameState(std::string stateID): id(stateID)
{
	//Pedimos a Ogre que cree un nuevo nodo hijo de la raiz para este nuevo estado
	stateNode = OgreManager::instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	//Borramos información anterior del puntero que usaran los componentes si existiera
	SceneManager::instance()->resetBuildStatePointer();
	//Guarda en el SceneManager el estado que se esta creando en el momento...
	//Para que el entityFactory al cargar (load) los componente cuelgen de él.
	//Y las llamadas sean a SceneManager y no a una variable local de EntityFactory
	SceneManager::instance()->buildingStateType(this);

	addEntities(EntityFactory::Instance()->createEntities(id));
}

	
GameState::~GameState()
{
	std::cout << "Destructora de GameState: " << id << std::endl;
	std::cout << "Entidades en entities: " << entities.size() << std::endl;
	//Borra todas las entidades de la escena
	removeAllEntities();
}

void GameState::start()
{
	for (Component* c : components)
		if (c->isActive())
			c->start();
}

void GameState::update(unsigned int time)
{
	PhysicsManager::instance()->stepSimulation(time); //FÍSICA

	//Actualizamos las entidades
	for (Component* c : components)
		if(c->isActive())
			c->update(time); 

	//Borramos las que lo hayan solicitado
	for (Entity* e : removedEntities)
		removeEntity(e);
	removedEntities.clear();
}

void GameState::render(unsigned int time)
{
	//OgreManager::instance()->render(time);
	for (Component* c : components)
		if (c->isActive())
			c->render(time);
}

bool GameState::handleInput(unsigned int time)
{
	bool handled = false;

	std::list<Component*>::iterator it = components.begin();
	while (it != components.end() && !handled) {
		if ((*it)->isActive())
			handled = (*it)->handleEvent(time);
		it++;
	}

	return false;
}

void GameState::addEntity(Entity* e)
{
	//Añadimos la entidad...
	entities.push_back(e);
	//Y sus componentes
	for (Component* c : e->getComponents()) 
		components.push_back(c);
}

void GameState::addEntities(std::vector<Entity*> ent)
{
	for (Entity* e : ent)
		addEntity(e);
}

bool GameState::removeAllEntities() {
	for (Entity* e : entities) {
		removedEntities.push_back(e);
	}

	for (Entity* e : removedEntities) {
		removeEntity(e);
	}

	removedEntities.clear();
	entities.clear();

	return true;
}


bool GameState::removeEntity(std::string name)
{
	//Cogemos la entidad
	Entity* e = getEntity(name);

	if (e == nullptr) //Si no está, devolvemos false
		return false;

	//La borraremos luego
	removedEntities.push_back(e);
	return true;
}

void GameState::removeEntity(Entity* e)
{
	//Borramos todos sus componentes...
	for (Component* c : e->getComponents())
	{
		components.remove(c); //Lo quitamos de la escena
		e->delComponent(c); //Se lo quitamos a la entidad
		delete c; //Lo eliminamos
		c = nullptr;
	}

	//La borramos a ella
	entities.remove(e);
	delete e;
	e = nullptr;
}

Entity* GameState::getEntity(std::string name)
{
	Entity* e = nullptr;
	std::list<Entity*>::iterator it = entities.begin();

	// Buscamos al primer componente que tenga esa entidad
	while (it != entities.end() && e == nullptr)
	{
		if ((*it)->getName() == name)
			e = (*it);
		it++;
	}

	return e;
}

std::list<Component*> GameState::getComponents()
{
	return components;
}

Ogre::SceneNode * GameState::getStateNode()
{
	//std::cout << "--getStateNode Called--" << std::endl;
	//info();
	return stateNode;
}

void GameState::info() {
	std::cout << " * ID: " << id << std::endl;
	std::cout << " * StateNode: ";
	if (stateNode == nullptr) std::cout << " NULLPTR " << std::endl;
	else std::cout << stateNode->getName() << std::endl;
	std::cout << " - ENTITY LIST w/ Components - " << std::endl;
	for (Entity* e : entities) { 
		std::cout << " * " << e->getName() << std::endl; 
		for (Component* c : e->getComponents()) {
			std::cout << "  + " << c->getInfo() << std::endl;
		}
	}
	std::cout << std::endl;

}