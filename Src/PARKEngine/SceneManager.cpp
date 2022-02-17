#include "SceneManager.h"
#include "ResourceManager.h"
#include "HUDManager.h"

std::unique_ptr<SceneManager> SceneManager::instance_;


void SceneManager::initInstance(bool* ex)
{
	if (instance_.get() == nullptr)
		instance_.reset(new SceneManager(ex));
}

SceneManager* SceneManager::instance()
{
	assert(instance_.get() != nullptr);
	return instance_.get();
}

SceneManager::SceneManager(bool* ex)
{
	exitedPlayState = false;
	exit = ex;
}


void SceneManager::disableOtherStatesNodes()
{
	for (auto s = states.begin(); s != states.end(); s++) {
		(*s).second->getStateNode()->setVisible((*s).second == currentState_);
	}
}

SceneManager::~SceneManager()
{
	std::cout << "Destructora de SceneManager" << std::endl;
	auto it = states.begin();
	while (it != states.end())
	{
		delete (*it).second;
		(*it).second = nullptr;
		++it;
	}
		
	states.clear();
	instance_.release();
}
			

GameState* SceneManager::currentState()
{
	return currentState_;
}

void SceneManager::resetBuildStatePointer() {
	//BuildingState es un puntero auxiliar para que
	//los componentes se cargen sobre el nuevo estado
	//Que esta generando el EntityFactory
	//Tenemos que reiniciarlo cuando acabe de crearse el nuevo estado
	if (buildingState_ != nullptr) {
		//IMPORTANTE! No borramos lo apuntado por el puntero (delete)
		//Eso borraría la información que se guarda en la variable
		//local STATE de SceneManager::addState()
		buildingState_ = nullptr;
	}
}

void SceneManager::buildingStateType(GameState* state)
{
	//Apunta al estado que esta generandose y al EntityFactory que lo rellena.
	buildingState_ = state;
}

GameState* SceneManager::buildingState()
{
	return buildingState_;
}

GameState* SceneManager::addState(std::string stateID)
{
	//State es una variable local de este método, para hacerlo accesible desde fuera
	//tenemos un puntero no local apuntara a cada nuevo state que deseemos crear
	//se inicializará en la constructora de GameState y liberará al final del addState
	GameState* state = nullptr;

	//Solo lo añadimos si no está ya
	auto it = states.find(stateID);
	if(it == states.end())
	{
		//Primero añadimos una ventana y creamos el estado
		HUDManager::instance()->addWindow(stateID);
		HUDManager::instance()->changeWindow(stateID);
		//Importante: Al crear un nuevo estado su constructora
		//inicializara la variable buildingState_ del SceneManager.
		state = new GameState(stateID);

		//Añadimos el estado a nuestro mapa de estados
		states.insert(std::pair<std::string, GameState*>(stateID, state));

		//Volvemos a la ventana en la que estábamos
		if(currentState_ != nullptr)
			HUDManager::instance()->changeWindow(currentState_->getID());
	}

	//Limpiamos información del puntero auxiliar una vez creado
	//el estado y cargado sobre el los componentes
	resetBuildStatePointer();

	debugInfo("AddState", stateID);

	//devolvemos el nuevo estado creado
	return state;
}

void SceneManager::changeState(std::string stateID)
{

	if (currentState_ != nullptr && currentState_->getID() == "StatePlay" && stateID != "StatePlay") {
		exitedPlayState = true;
	}

	HUDManager::instance()->changeWindow(stateID);

	auto it = states.find(stateID);
	assert(it != states.end());
	currentState_ = it->second;
	currentState_->start();

	debugInfo("changeState", stateID);

	disableOtherStatesNodes();
}

bool SceneManager::removeState(std::string stateID) {
	//Generamos un iterador para que busque el estado deseado
	auto it = states.find(stateID);

	//Si no lo ha encontrado (Posición final)
	if (it == states.end())
		return false;
	else {
		delete it->second;
		states.erase(it);
		return true;
	}

	debugInfo("removeState", stateID);
}

bool SceneManager::hasExitPlay() {
	return exitedPlayState;
}

void SceneManager::resetExitedPlay() {
	exitedPlayState = false;
}

void SceneManager::debugInfo(std::string msg, std::string stateID) {
	std::map <std::string, GameState*> statesAux;

	std::cout << "|||| Scene Manager Debug Info ||||" << std::endl;
	if (msg.size() != 0) std::cout << "Called from method: " << msg << " using stateID: " << stateID << std::endl;

	if (currentState_ != nullptr) std::cout << "Current State: " << currentState_->getID() << std::endl;
	else std::cout << "Current State: NULLPTR" << std::endl;
	std::cout << "States Map size: " << states.size() << std::endl;
}