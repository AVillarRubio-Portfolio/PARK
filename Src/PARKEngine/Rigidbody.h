#pragma once
#include "Component.h"


class Transform;
enum Shape;
class btVector3;
class btRigidBody;
class btTransform;

class Rigidbody : public Component
{
public:
	Rigidbody();

	//Constructoras para Rigidbodys asociados a una malla
	Rigidbody(Transform* transform, Shape shape, float mass = 1);

	~Rigidbody();

	virtual void start();
	
	virtual void load(json file);

	//Generic methods
	virtual void update(unsigned int time);

	virtual void save(json& file) {};

	//API
	//Add a force to the rigidBody
	virtual void addForce(Vector3 force);
	//Add a torque to the rigidbody
	virtual void addTorque(Vector3 torque);
	//Clears all current forces on the rigidbody
	virtual void clearForces();


	//Getters
	//Returns bounding box of the rigidbody, especified by minimum and maximum vectors
	virtual void getAABB(Vector3& min, Vector3& max);
	//Returns mass of the rigidbody
	virtual float getMass();

	//Returns linear velocity of the rigidbody
	virtual Vector3 getLinearVelocity();
	//Returns angular velocity of the rigidbody
	virtual Vector3 getAngularVelocity();


	//Returns total force of the rigidbody
	virtual Vector3 getTotalForce();
	//Returns total torque of the rigidbody
	virtual Vector3 getTotalTorque();

	//Tells whether the object is kinematic or not
	virtual bool isKinematic();
	//Tells whether the object is static or not
	virtual bool isStatic();


	//Setters
	//Sets linear velocity of the rigidbody
	virtual void setLinearVelocity(Vector3 linearVel);
	//Sets angular velocity of the rigidbody
	virtual void setAngularVelocity(Vector3 angVel);

	//Position
	//Returns rigidbody's position
	virtual Vector3 getPosition();
	//Sets rigidbody's position
	virtual void setPosition(Vector3 destiny);

	//Position
	//Returns rigidbody's position
	virtual Transform* getTransform();
	//Sets rigidbody's position
	virtual void setTransform(Transform* transform);

	//Restitution
	//Returns rigidbody's position
	virtual float getRestitution();
	//Sets rigidbody's position
	virtual void setRestitution(float restitution);

	//Freeze position/rotation
	//Allows the rigidbody to move only in the specified axis
	virtual void setLinearFactor(Vector3 axis);
	//Freezes rotation in the given axis
	virtual void setAngularFactor(Vector3 axis);

	virtual void activate();

	//Returns a string with indented information of the rigidbody
	virtual std::string getInfo();
	virtual virtual std::string getDetailedInfo();


protected:
	btRigidBody* rigid_;
	Transform* transform_;
	float mass_;
	Vector3* dims_;

	virtual btTransform getBtTransform();
	virtual void updateTransform();
};

REGISTER_TYPE(Rigidbody)