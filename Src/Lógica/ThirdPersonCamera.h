#pragma once
#include "PARKEngine/Component.h"

class Camera;
class Transform;
class Matrix;
class Rigidbody;

class ThirdPersonCamera : public Component
{
public:
	ThirdPersonCamera();
	~ThirdPersonCamera();


	virtual void load(json file);

	virtual void start();

	virtual void receive(Message * msg);
	
	virtual bool handleEvent(unsigned int time);
	virtual std::string getInfo() { return "ThirdPersonCamera"; };

protected:
	//References to Camera's components
	Camera * cam_;
	Transform* camTransform_;
	Rigidbody* camRigid_;

	//Moves the camera a certain increment
	virtual void moveCamera(Vector3 deltaPos);

	////Makes the camera orbit around its focus a certain degree amount
	virtual void orbit(float degrees);

	//To help restraint camera movement within the matrix
	float MIN_HEIGTH;
	float MAX_HEIGTH;

	//Fraction of the screen used to move the camera
	float borders_;

	bool changedCamera_;
	bool rotating_;

	Vector3 defaultPos_;
};

REGISTER_TYPE(ThirdPersonCamera)