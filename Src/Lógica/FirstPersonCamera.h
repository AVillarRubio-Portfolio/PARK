#pragma once
#include "PARKEngine/Component.h"

class Camera;
class Transform;
class Rigidbody;

class FirstPersonCamera : public Component
{
public:
	FirstPersonCamera();
	~FirstPersonCamera();


	virtual void load(json file);

	virtual void start();

	virtual void receive(Message * msg);
	virtual bool handleEvent(unsigned int time);
	virtual std::string getInfo() { return "FirstPersonCamera"; };

protected:
	//References to Camera's components
	Camera * cam_;
	Transform* camTransform_;
	Rigidbody* camRigid_;

	Vector3 entrance_;
	bool changedCamera_;
};

REGISTER_TYPE(FirstPersonCamera)