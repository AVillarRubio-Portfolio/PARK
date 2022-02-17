#include "Game.h"

//NUESTRO
#include <PARKEngine/PARKManagers.h>
#include <PARKEngine/PARKFoundation.h>

#include "SDL.h"


Game::Game(std::string basicConfig):exit(false)
{
	//Inicializamos singletons
	OgreManager::initInstance(basicConfig);
	//ResourceManager::initInstance();// -> se hace en el OgreManager
	InputManager::initInstance("Input.json");
	HUDManager::initInstance();
	AudioManager::initInstance("AudioSource.json");
	SceneManager::initInstance(&exit);
	PhysicsManager::initInstance();
	CallbackManager::initInstance();

	//Cogemos las instancias en las variables
	ogreManager_ = OgreManager::instance();
	inputManager_ = InputManager::instance();
	hudManager_ = HUDManager::instance();
	audioManager_ = AudioManager::instance();
	sceneManager_ = SceneManager::instance();
	physicsManager_ = PhysicsManager::instance();
}

Game::~Game()
{
	std::cout << "Destructora de Game" << std::endl;
	if (physicsManager_ != nullptr)
		delete physicsManager_;
	if (sceneManager_ != nullptr)
		delete sceneManager_;
	if (audioManager_ != nullptr)
		delete audioManager_;
	if (inputManager_ != nullptr)
		delete inputManager_;
	if (ogreManager_ != nullptr)
		delete ogreManager_;
}

void Game::start()
{
	//Añadimos todos los estados
	sceneManager_->addState("StateMainMenu");
	sceneManager_->addState("StateOptions");
	sceneManager_->addState("StatePlay");

	//Cambiamos al actual
	sceneManager_->changeState("StateMainMenu");

	audioManager_->PLAY_SONG("Menu");
}

void Game::run()
{
	//Tiempo entre frames y tiempo total transcurrido
	unsigned int deltaTime, actualTime;
	actualTime = deltaTime = SDL_GetTicks();
	//Bucle principal
	while (!exit)
	{
		//Llama al update, handleInput y render de la escena activa
		sceneManager_->currentState()->update(deltaTime);
		inputManager_->capture();
		sceneManager_->currentState()->handleInput(deltaTime);
		ogreManager_->render(deltaTime);

		//Actualiza el deltaTime
		deltaTime = SDL_GetTicks() - actualTime;
		actualTime = SDL_GetTicks();	
	}
}
