#pragma once
#include "Component.h"

class Transform;
class Ogre::Camera;
class Ogre::SceneNode;

class Camera : public Component
{
public:
	Camera();
	~Camera();

	virtual void start();

	virtual void update(unsigned int time);

	virtual void receive(Message* msg);

	virtual void save(json& file);
	//Info
	virtual std::string getInfo() { return "Camera"; }

	virtual void lookAt(Vector3 pos);
protected:
	//Pointer to the transform component
	Transform* transform_;

	//Ogre camera
	Ogre::SceneNode* camNode_;
	Ogre::Camera *camera_;
};

REGISTER_TYPE(Camera)