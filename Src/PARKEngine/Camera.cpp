#include "Camera.h"

#include "Entity.h"
#include "Transform.h"

#include "OgreManager.h"
#include "ResourceManager.h"
#include "InputManager.h"

#include "OgreIncludes.h"


#include <CEGUI/CEGUI.h>
#include <SDL_video.h>

#include <OISKeyboard.h>
#include <OISMouse.h>


Camera::Camera()
{
}


Camera::~Camera()
{
	std::cout << "Destructora de Camera" << std::endl;
}

void Camera::start()
{
	//Camera already created
	if (OgreManager::instance()->getCamera() != nullptr)
		camera_ = OgreManager::instance()->getCamera();
	//Create a new one
	else
		camera_ = OgreManager::instance()->createCamera(getEntity()->getName(), 5, 50000, true);

	camNode_ = (Ogre::SceneNode*)camera_->getParentNode();
	camera_->getViewport()->setClearEveryFrame(true);

	//Initialises it 
	transform_ = entity_->getComponent<Transform>();
	assert(transform_ != nullptr);

	//Initialise camNode
	Vector3 pos = transform_->getPosition(); Quaternion orientation = transform_->getRotation();
	camNode_->setPosition(pos);
	camNode_->setOrientation(orientation);
}

void Camera::update(unsigned int time)
{
}

void Camera::receive(Message* msg)
{
	if(msg->mType_ == MessageId::TRANSFORM_CHANGED)
	{
		camNode_->setPosition(transform_->getPosition());
		camNode_->setOrientation(transform_->getRotation());
	}
}

void Camera::save(json& file)
{
}

void Camera::lookAt(Vector3 pos)
{
	camNode_->lookAt(pos, Ogre::Node::TS_WORLD);
	transform_->setRotation(camNode_->getOrientation());
}
