#include "CallbackManager.h"
#include "Lógica/BureaucracyManager.h"
#include "Lógica/ConstructionMode.h"
#include "Lógica/DatosEdificio.h"
#include "Entity.h"
#include "Camera.h"

#include "SceneManager.h"
#include "AudioManager.h"
#include "TextBox.h"
#include <CEGUI/Window.h>
#include "FrameWindowBox.h"

using namespace std::placeholders;

std::unique_ptr<CallbackManager> CallbackManager::instance_;


CallbackManager::CallbackManager()
{
	initCallbacks();
}
CallbackManager::~CallbackManager()
{
	std::cout << "Destructora de CallbackManager" << std::endl;
}

void CallbackManager::initInstance()
{
	//Crea una instancia nueva
	if (instance_.get() == nullptr)
		instance_.reset(new CallbackManager());
}

CallbackManager* CallbackManager::instance()
{
	//Devuelve la instancia
	assert(instance_.get() != nullptr);
	return instance_.get();
}


std::function<void(std::string)> CallbackManager::getCallback(std::string name)
{
	return callbacks[name];
}

template <typename T>
void CallbackManager::addCallback(std::string keyName, bool(T::*cb)(std::string), T* obj)
{
	callbacks[keyName] = std::bind(cb, obj, _1);
}


void CallbackManager::initCallbacks()
{
	addCallback("onChangeState", &CallbackManager::onChangeState, this);
	addCallback("onExit", &CallbackManager::onExit, this);
	addCallback("construct", &CallbackManager::construct, this);
	addCallback("setDeleteBuildingActive", &CallbackManager::setDeleteBuildingActive, this);
	addCallback("EffectVolumeChange", &CallbackManager::EffectVolumeChange, this);
	addCallback("MusicVolumeChange", &CallbackManager::MusicVolumeChange, this);

	// Para los paneles
	//Panel de las herramientas de Construccion y Gestion
	addCallback("setToolsPanelActive", &CallbackManager::setConstructPanelActive, this);

	//Paneles de Construccion y Gestion
	addCallback("setConstructPanelActive", &CallbackManager::setConstructPanelActive, this);
	addCallback("setInfoPanelActive", &CallbackManager::setInfoPanelActive, this);

	// Panel con la informacion de los NPCs
	addCallback("setNPCInfoPanelActive", &CallbackManager::setNPCInfoPanelActive, this);

	//Paneles de Tipos de Construccion
	addCallback("setRoadConstructionActive", &CallbackManager::setRoadConstructionActive, this);
	addCallback("setAmusementsConstructionActive", &CallbackManager::setAmusementsConstructionActive, this);
	addCallback("setRestaurantsConstructionActive", &CallbackManager::setRestaurantsConstructionActive, this);
	addCallback("setToiletsConstructionActive", &CallbackManager::setToiletsConstructionActive, this);

}



// -------------- CALLBACKS -------------- //


bool CallbackManager::onExit(std::string null)
{
	SceneManager::instance()->exitGame();
	return true;
}

bool CallbackManager::onChangeState(std::string state)
{
	//Asumimos que hemos añadido todos los estados al principio
	SceneManager::instance()->changeState(state);

	//La musiquita está bien hasta que llevas escuchándola 3 horas
#if !_DEBUG
	if(state == "StatePlay")
	{
		AudioManager::instance()->STOP_SOUND("Menu");
		AudioManager::instance()->PLAY_SONG("MainTheme");
	}
	else if (state == "StateMainMenu")
	{
		AudioManager::instance()->STOP_SOUND("MainTheme");
		AudioManager::instance()->PLAY_SONG("Menu");
	}
#endif
	
	return true;
}

bool CallbackManager::EffectVolumeChange(std::string vol)
{
	if (vol == "+") AudioManager::instance()->UP_EFFECTS_VOLUME();
	else if (vol == "-") AudioManager::instance()->DOWN_EFFECTS_VOLUME();
	SceneManager::instance()->currentState()->getEntity("TextVolumeEffects")->getComponent<TextBox>()->setText(std::to_string((int)AudioManager::instance()->getSoundVolume()));
	return true;
}

bool CallbackManager::MusicVolumeChange(std::string vol)
{
	if (vol == "+") AudioManager::instance()->UP_MUSIC_VOLUME();
	else if (vol == "-") AudioManager::instance()->DOWN_MUSIC_VOLUME();
	SceneManager::instance()->currentState()->getEntity("TextVolumeMusic")->getComponent<TextBox>()->setText(std::to_string((int)AudioManager::instance()->getMusicVolume()));
	return true;
}

bool CallbackManager::construct(std::string buildName)
{

	BureauCrazyManager* bureauCrazyManager_ = SceneManager::instance()->currentState()->getEntity("BureauCrazyManager")->getComponent<BureauCrazyManager>();
	ConstructionMode* constructionMode = SceneManager::instance()->currentState()->getEntity("ConstructionMode")->getComponent<ConstructionMode>();
	DatosEdificio* datosEdificio = SceneManager::instance()->currentState()->getEntity("DatosEdificios")->getComponent<DatosEdificio>();
	if (bureauCrazyManager_->getActualMoney() >= datosEdificio->getPrice(buildName)) {
		if (datosEdificio->getLocked(buildName)) {
			bureauCrazyManager_->addMoney(-datosEdificio->getPrice(buildName));
			datosEdificio->setLocked(buildName, false);
		}
		else {
			constructionMode->construct(buildName);
		}
	}
	return true;
}

bool CallbackManager::setDeleteBuildingActive(std::string s)
{
	ConstructionMode* constructionMode = SceneManager::instance()->currentState()->getEntity("ConstructionMode")->getComponent<ConstructionMode>();
	constructionMode->setDeleteActive(!constructionMode->getDeleteActice());
	if (constructionMode->getDeleteActice())
		SceneManager::instance()->currentState()->getEntity("TextDelete")->getComponent<TextBox>()->getStaticText()->show();
	else
		SceneManager::instance()->currentState()->getEntity("TextDelete")->getComponent<TextBox>()->getStaticText()->hide();

	return true;
}


//PANEL DE LAS HERRAMIENTAS
bool CallbackManager::setToolsPanelActive(std::string boolean)
{
	FrameWindowBox* w = SceneManager::instance()->currentState()->getEntity("ToolsPanel")->getComponent<FrameWindowBox>();
	if (boolean == "True")
		w->show();
	else
		w->hide();

	return true;
}

//PANEL DE CONSTRUCCION
bool CallbackManager::setConstructPanelActive(std::string boolean)
{
	FrameWindowBox* w = SceneManager::instance()->currentState()->getEntity("ConstructPanel")->getComponent<FrameWindowBox>();
	if (boolean == "True") {
		w->show();
		setToolsPanelActive("False");
	}
	else {
		w->hide();
		setToolsPanelActive("True");
	}

	return true;
}

//PANEL DE GESTION
bool CallbackManager::setInfoPanelActive(std::string boolean)
{
	FrameWindowBox* w = SceneManager::instance()->currentState()->getEntity("InfoPanel")->getComponent<FrameWindowBox>();
	if (boolean == "True") {
		w->show();
		setToolsPanelActive("False");
	}
	else {
		w->hide();
		setToolsPanelActive("True");
	}

	return true;
}

bool CallbackManager::setNPCInfoPanelActive(std::string boolean)
{
	FrameWindowBox* w = SceneManager::instance()->currentState()->getEntity("NPCInfoPanel")->getComponent<FrameWindowBox>();
	if (boolean == "True") {
		w->show();
	}
	else {
		w->hide();
	}

	return true;
}

//PANEL DE LOS TIPOS DE CARRETERA
bool CallbackManager::setRoadConstructionActive(std::string boolean)
{
	FrameWindowBox* w = SceneManager::instance()->currentState()->getEntity("RoadsPanel")->getComponent<FrameWindowBox>();
	FrameWindowBox* c = SceneManager::instance()->currentState()->getEntity("ConstructPanel")->getComponent<FrameWindowBox>();
	if (boolean == "True") {
		w->show();
		c->hide();
	}
	else {
		w->hide();
		c->show();
	}

	return true;
}

// PANEL DE LOS TIPOS DE ATRACCIONES
bool CallbackManager::setAmusementsConstructionActive(std::string boolean)
{
	FrameWindowBox* w = SceneManager::instance()->currentState()->getEntity("AmusementsPanel")->getComponent<FrameWindowBox>();
	FrameWindowBox* c = SceneManager::instance()->currentState()->getEntity("ConstructPanel")->getComponent<FrameWindowBox>();
	if (boolean == "True") {
		w->show();
		c->hide();
	}
	else {
		w->hide();
		c->show();
	}

	return true;
}

// PANEL DE LOS TIPOS DE RESTAURANTES
bool CallbackManager::setRestaurantsConstructionActive(std::string boolean)
{
	FrameWindowBox* w = SceneManager::instance()->currentState()->getEntity("RestaurantsPanel")->getComponent<FrameWindowBox>();
	FrameWindowBox* c = SceneManager::instance()->currentState()->getEntity("ConstructPanel")->getComponent<FrameWindowBox>();
	if (boolean == "True") {
		w->show();
		c->hide();
	}
	else {
		w->hide();
		c->show();
	}

	return true;
}

// PANEL DE LOS TIPOS DE BAÑOS
bool CallbackManager::setToiletsConstructionActive(std::string boolean)
{
	FrameWindowBox* w = SceneManager::instance()->currentState()->getEntity("ToiletsPanel")->getComponent<FrameWindowBox>();
	FrameWindowBox* c = SceneManager::instance()->currentState()->getEntity("ConstructPanel")->getComponent<FrameWindowBox>();
	if (boolean == "True") {
		w->show();
		c->hide();
	}
	else {
		w->hide();
		c->show();
	}

	return true;
}
