#include "BackgroundImage.h"

#include "OgreIncludes.h"
#include "OgreManager.h"
#include "EntityFactory.h"
#include "SceneManager.h"
#include "GameState.h"
#include "HUDManager.h"

BackgroundImage::BackgroundImage()
{
}

BackgroundImage::~BackgroundImage()
{
	std::cout << "Destructora de BackgroundImage" << std::endl;
}

void BackgroundImage::load(json file)
{
	std::string nombre;

	nombre = file["ImageName"].get<std::string>();

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(nombre + "material", "General");
	material->getTechnique(0)->getPass(0)->createTextureUnitState(nombre);
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);

	rect->setCorners(-1.0, 1.0, 1.0, -1.0);
	rect->setMaterial(material);


	rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
	

	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	rect->setBoundingBox(aabInf);

	//Ogre::SceneNode* node = EntityFactory::Instance()->get_currentState()->getStateNode()->createChildSceneNode();
	Ogre::SceneNode* node = SceneManager::instance()->buildingState()->getStateNode()->createChildSceneNode();
	node->attachObject(rect);
}
