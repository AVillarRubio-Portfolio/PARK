#include "OgreManager.h"

//NUESTRO
#include "PARKFoundation.h"
#include "PARKManagers.h"
#include "Utils.h"
#include "TerrainCreator.h"
#include "MeshRenderer.h"
#include "windows.h"
//#include "OgreWindowEventUtilities.h"

//OGRE
#include "OgreIncludes.h"

//Otros
#include <iostream>
#include "SDL.h"

//CEGUI
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>


std::unique_ptr<OgreManager> OgreManager::instance_;

void OgreManager::initInstance(std::string initFileJson)
{
	//Devuelve la instancia si exise, si no crea una nueva
	if (instance_.get() == nullptr)
		instance_.reset(new OgreManager(initFileJson));
}

OgreManager * OgreManager::instance()
{
	assert(instance_.get() != nullptr);
	return instance_.get();
}


OgreManager::OgreManager(std::string initFileJson):plane_(nullptr), camera_(nullptr)
{	

#if _DEBUG
	root_ = new Ogre::Root("plugins_d.cfg");
#else 	
	root_ = new Ogre::Root("plugins.cfg");
#endif

	root_->setRenderSystem(*(root_->getAvailableRenderers().begin()));
	root_->initialise(false);

	//Inicializa el ResourceManager
	ResourceManager::initInstance();

	//Esto antes estaba en el ResourceManager
	getTextureManager()->setDefaultNumMipmaps(5);
	getResourceGroupManager()->initialiseAllResourceGroups();

	sceneMgr_ = root_->createSceneManager();

	//std::cout << "InitFileJson : " << initFileJson << std::endl;
	initWindow(initFileJson);		

	//Inicialización de ventana de SDL que se una a la de Ogre
	SDL_Init(SDL_INIT_EVERYTHING);
	hWnd = 0;
	window_->getCustomAttribute("WINDOW", &hWnd);
	SDL_CreateWindowFrom((void*)hWnd);
}


OgreManager::~OgreManager()
{
	std::cout << "Destructora de OgreManager" << std::endl;
	CEGUI::System::destroy();
	
	if (root_ != nullptr){
		std::cout << "Deleting Ogre root_. This takes a while, please wait" << std::endl;
		delete root_;
	}
	if (plane_ != nullptr)
		delete plane_;

	instance_.release();
}


void OgreManager::messagePump() {
	MSG  msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void OgreManager::render(unsigned int deltaTime)
{
	if (window_->isActive()) {
		messagePump();	//Importante para detectar llamadas del sistema operativo sobre la ventana (redimensionado, foco, minimizar, etc)
		sceneMgr_->_updateSceneGraph(camera_);
		root_->renderOneFrame((Ogre::Real)deltaTime / 1000);	
		OIS::Mouse* mouse =  InputManager::instance()->getMouse();
	
		HUDManager::instance()->injectMouseMove(mouse->getMouseState().X.rel
			, mouse->getMouseState().Y.rel );

		HUDManager::instance()->injectTimePulse(deltaTime / 1000.0f);
	}
	else {
		messagePump();
		root_->clearEventTimes();
	}
}

Ogre::SceneManager * OgreManager::getSceneManager()
{
	return sceneMgr_;
}

Ogre::MeshManager * OgreManager::getMeshManager()
{
	return &Ogre::MeshManager::getSingleton();
}

Ogre::ResourceGroupManager * OgreManager::getResourceGroupManager()
{
	return &Ogre::ResourceGroupManager::getSingleton();
}

Ogre::TextureManager * OgreManager::getTextureManager()
{
	return &Ogre::TextureManager::getSingleton();
}

std::pair<Entity*, Ogre::Vector3> OgreManager::raycastToMouse(std::string ignoreEntityName)
{
	return raycast(HUDManager::instance()->getMouseCursor().getPosition().d_x /
		float(OgreManager::instance()->getWindow()->getWidth()),
		HUDManager::instance()->getMouseCursor().getPosition().d_y /
		float(OgreManager::instance()->getWindow()->getHeight()), ignoreEntityName);
}

std::pair<Entity*, Ogre::Vector3> OgreManager::raycast(float screenX, float screenY, std::string ignoreEntityName)
{
	Ogre::RaySceneQuery* m_pray_scene_query = sceneMgr_->createRayQuery(Ogre::Ray(), sceneMgr_->WORLD_GEOMETRY_TYPE_MASK);
	if (nullptr == m_pray_scene_query) return (std::pair<Entity* ,Ogre::Vector3>(nullptr,Ogre::Vector3::ZERO));
	m_pray_scene_query->setSortByDistance(true);
	Ogre::Ray ray;
	// check we are initialised
	if (m_pray_scene_query != nullptr)
	{
		// create a query object
		ray = camera_->getCameraToViewportRay(screenX, screenY);
		m_pray_scene_query->setRay(ray);
	}

	// at this point we have raycast to a series of different objects bounding boxes.
	// we need to test these different objects to see which is the first polygon hit.
	// there are some minor optimizations (distance based) that mean we wont have to
	// check all of the objects most of the time, but the worst case scenario is that
	// we need to test every triangle of every object.
	Ogre::Real closest_distance = -1.0f;
	Ogre::Vector3 closest_result;
	Ogre::RaySceneQueryResult& query_result = m_pray_scene_query->execute();

	//ray.getOrigin+ray.getDirection()*result.second
	for (int qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
	{
		// stop checking if we have found a raycast hit that is closer
		// than all remaining entities
		if ((closest_distance >= 0.0f) &&
			(closest_distance < query_result[qr_idx].distance))
		{
			break;
		}

		// only check this result if its a hit against an entity
		if ((query_result[qr_idx].movable != NULL) &&
			(query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
		{
			// get the entity to check
			Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);
			Entity* en = getEntityFromNode(pentity->getParentSceneNode());
			if (ignoreEntityName == "" || (ignoreEntityName != "" && en->getName() != ignoreEntityName)) {
				std::pair<Entity*, Ogre::Vector3> pair_(en, ray.getPoint(query_result[qr_idx].distance));
				//pentity->setVisible(false);
				return pair_;
			}
		}
	}

	return (std::pair<Entity*, Ogre::Vector3>(nullptr, Ogre::Vector3::ZERO));
}


void OgreManager::initWindow(std::string initFileJson)
{
	json initFile = ResourceManager::instance()->getJsonByKey(initFileJson);

	//Ventana
	Ogre::NameValuePairList options;
	//options["left"] = "0";
	//options["top"] = "0";
	options["border"] = "none";
	options["monitorIndex"] = "0";

	if (initFile != nullptr){
		window_ = root_->createRenderWindow(initFile["WindowName"], initFile["Width"], initFile["Height"], false, &options);
		window_->setFullscreen(initFile["fullScreen"], initFile["Width"], initFile["Height"]);
	}
	else {
		window_ = root_->createRenderWindow("PARK - No InitAplication File Found", 720, 480, false, &options);
		window_->setFullscreen(false, 720, 480);
	}

	//LUZ
	sceneMgr_->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
	Vector3 lightdir(0.55, -0.3, 0.75);
	lightdir.normalise();

	light_ = sceneMgr_->createLight("TestLight");
	light_->setType(Ogre::Light::LT_DIRECTIONAL);
	light_->setDirection(lightdir);
	light_->setDiffuseColour(Ogre::ColourValue::White);
	light_->setSpecularColour(Ogre::ColourValue(240 / 255, 240 / 255, 188 / 255));

	//Plano Skyplane
	plane_ = new Ogre::Plane();
	plane_->d = 1000;
	plane_->normal = Vector3::NEGATIVE_UNIT_Y;
	sceneMgr_->setSkyPlane(
		true, *plane_, "SkyBox", 1500, 50, true, 1.5, 150, 150);


#if _DEBUG
	sceneMgr_->showBoundingBoxes(true); //Para debuggear las aabb
#endif
}

float OgreManager::getWindowSizeX()
{
	return viewport_->getActualWidth();
}

float OgreManager::getWindowSizeY()
{
	return viewport_->getActualHeight();
}

Ogre::Camera* OgreManager::createCamera(std::string name, float NearClipDist, float FarClipDist, bool autoAspectRatio, float AspectRatio)
{
	//Create Ogre node
	Ogre::SceneNode* camNode_ = sceneMgr_->getRootSceneNode()->createChildSceneNode("camNode");

	//Create Ogre camera
	camera_ = sceneMgr_->createCamera(name);
	camera_->setNearClipDistance(NearClipDist);
	camera_->setFarClipDistance(FarClipDist);

	//Attach object to the camera
	camNode_->attachObject(camera_);
	if (autoAspectRatio)
		camera_->setAutoAspectRatio(autoAspectRatio);
	else
		camera_->setAspectRatio(AspectRatio);

	//Update actual camera and viewport
	viewport_ = window_->addViewport(camera_);

	return camera_;
}

Ogre::Entity * OgreManager::createPlane(std::string name, std::string MaterialName, float width, float height, int Xsegments, int Ysegments, Ogre::SceneNode* FatherNode)
{
	Ogre::Entity* plane;
	getMeshManager()->createPlane(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Vector3::UNIT_Y, 0), width, height,
		Xsegments, Ysegments, true, 1, 1.0, 1.0, Vector3::NEGATIVE_UNIT_Z);
	plane = getSceneManager()->createEntity(name);
	plane->setMaterialName(MaterialName);
	FatherNode->attachObject(plane);
	return plane;
}

Entity * OgreManager::getEntityFromNode(Ogre::SceneNode * n)
{
	for (Entity* c : SceneManager::instance()->currentState()->getEntitiesWithComponent<MeshRenderer>())
	{
		if (c->getComponent<MeshRenderer>()->getNode() == n)return c;
	}
}

TerrainGenerator * OgreManager::createTerrain(std::string terrainFile)
{
	TerrainGenerator* terrainCreator_ = new TerrainGenerator(getSceneManager(), getLight(), terrainFile);
	return terrainCreator_;
}

void OgreManager::deleteFileSystemLayer(Ogre::FileSystemLayer * fsLayer)
{
	OGRE_DELETE fsLayer;
}
