#include "Rigidbody.h"

#include "Entity.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "PhysicsManager.h"
#include <iostream>
#include "btBulletDynamicsCommon.h"



Rigidbody::Rigidbody() : transform_(nullptr), rigid_(nullptr), mass_(0), dims_(nullptr)
{
}


Rigidbody::Rigidbody(Transform* transform, Shape shape, float mass) : transform_(transform)
{
	//Creamos el rigidbody
	rigid_ = PhysicsManager::createRigidBody(shape, transform_->getScale(), mass);
}


void Rigidbody::load(json file)
{
	addParameter(mass_, file["mass"]);

	if(file.contains("dimensions"))
	{
		json dimensions = file["dimensions"];
		dims_ = new Vector3(dimensions["x"], dimensions["y"], dimensions["z"]);
	}
}

void Rigidbody::start()
{
	MeshRenderer* renderer = entity_->getComponent<MeshRenderer>();
	//Create the rigidbody from mesh
	if (dims_ == nullptr && renderer != nullptr)
	{
		Vector3 aabbMin, aabbMax; renderer->getAABB(aabbMin, aabbMax); //¿CUANDO TIENE VALOR VALIDO?
		dims_ = new Vector3(aabbMax - aabbMin);
	}
	else if (dims_ == nullptr)
		dims_ = new Vector3(1, 1, 1);

	rigid_ = PhysicsManager::createRigidBody(BoxShape, *dims_, mass_);

	//Set position
	transform_ = entity_->getComponent<Transform>();
	assert(transform_ != nullptr);
	rigid_->setUserPointer(transform_);
	

	//Colocamos en un inicio al rigidbody en el lugar del Transform de la entidad
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(transform_->getPosition().x, transform_->getPosition().y, transform_->getPosition().z));

	rigid_->setWorldTransform(trans);
}

Rigidbody::~Rigidbody()
{
	std::cout << "Destructora de RigidBody" << std::endl;
	delete dims_;
	dims_ = nullptr;
}

void Rigidbody::update(unsigned int time)
{
	updateTransform();
}

void Rigidbody::addForce(Vector3 force)
{
	rigid_->applyCentralImpulse(btVector3(force.x, force.y, force.z));
}

void Rigidbody::addTorque(Vector3 torque)
{
	rigid_->applyTorque(btVector3(torque.x, torque.y, torque.z));
}

void Rigidbody::clearForces()
{
	rigid_->clearForces();
}

void Rigidbody::getAABB(Vector3& min, Vector3& max)
{
	btVector3 btMin, btMax;
	rigid_->getAabb(btMin, btMax);

	//Actualizamos valores
	min = Vector3(btMin.getX(), btMin.getY(), btMin.getZ());
	max = Vector3(btMax.getX(), btMax.getY(), btMax.getZ());
}

Vector3 Rigidbody::getAngularVelocity()
{
	btVector3 angVel = rigid_->getAngularVelocity();
	return Vector3(angVel.getX(), angVel.getY(), angVel.getZ());
}

Vector3 Rigidbody::getLinearVelocity()
{
	btVector3 linearVel = rigid_->getLinearVelocity();
	return Vector3(linearVel.getX(), linearVel.getY(), linearVel.getZ());
}

float Rigidbody::getMass()
{
	return 1.0/rigid_->getInvMass();
}

Vector3 Rigidbody::getTotalForce()
{
	btVector3 totalForce = rigid_->getTotalForce();
	return Vector3(totalForce.getX(), totalForce.getY(), totalForce.getZ());
}


Vector3 Rigidbody::getTotalTorque()
{
	btVector3 totalTorque = rigid_->getTotalTorque();
	return Vector3(totalTorque.getX(), totalTorque.getY(), totalTorque.getZ());
}

bool Rigidbody::isKinematic()
{
	return rigid_->isKinematicObject();
}

bool Rigidbody::isStatic()
{
	return rigid_->isStaticObject();
}

void Rigidbody::setLinearVelocity(Vector3 linearVel)
{
	rigid_->setLinearVelocity(btVector3(linearVel.x, linearVel.y, linearVel.z));
}

void Rigidbody::setAngularVelocity(Vector3 angularVel)
{
	rigid_->setAngularVelocity(btVector3(angularVel.x, angularVel.y, angularVel.z));
}

Vector3 Rigidbody::getPosition()
{
	btTransform trans = getBtTransform();
	return Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
}

void Rigidbody::setPosition(Vector3 destiny)
{
	//Get actual transform and change position
	btTransform trans = getBtTransform();
	trans.setOrigin(btVector3(destiny.x, destiny.y, destiny.z));

	//Set it
	rigid_->setWorldTransform(trans);
	updateTransform();
}

Transform* Rigidbody::getTransform()
{
	btTransform btTrans = getBtTransform();
	Quaternion rot = btTrans.getRotation();

	Transform t = Transform(getPosition(), rot, transform_->getScale());
	return &t;
}

void Rigidbody::setTransform(Transform* transform)
{
	Vector3 pos = transform->getPosition();
	Quaternion rot = transform->getRotation();

	//Get actual transform and change position
	btTransform btTrans = getBtTransform();
	btTrans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	btTrans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));

	//Set it
	rigid_->setWorldTransform(btTrans);
}

float Rigidbody::getRestitution()
{
	return rigid_->getRestitution();
}

void Rigidbody::setRestitution(float restitution)
{
	rigid_->setRestitution(restitution);
}

void Rigidbody::setLinearFactor(Vector3 axis)
{
	rigid_->setLinearFactor(btVector3(axis.x, axis.y, axis.z));
}

void Rigidbody::setAngularFactor(Vector3 axis)
{
	rigid_->setAngularFactor(btVector3(axis.x, axis.y, axis.z));
}

void Rigidbody::activate()
{
	rigid_->activate();
}

btTransform Rigidbody::getBtTransform()
{
	btTransform trans;
	//Está en movimiento
	if (rigid_ && rigid_->getMotionState())
		rigid_->getMotionState()->getWorldTransform(trans);
	//Está parado (cogemos la posición inicial)
	else
		trans = rigid_->getWorldTransform();

	return trans;
}

void Rigidbody::updateTransform()
{
	//Obtenemos su posición y orientación
	btTransform trans = getBtTransform();

	//Actualizamos la posición y orientación del nodo de Ogre en función a las del Rigidbody
	transform_->setPosition(Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	transform_->setRotation(Quaternion(trans.getRotation().getW(), trans.getRotation().getX(),
		trans.getRotation().getY(), trans.getRotation().getZ()));
}

std::string Rigidbody::getInfo() {
	return "RigidBody -> More on getDetailedInfo()";
}

std::string Rigidbody::getDetailedInfo()
{
	Vector3 min, max;
	getAABB(min, max);
	Vector3 meshMin, meshMax;
	getBrotherComponent<MeshRenderer>()->getAABB(meshMin, meshMax);
	std::string s = " ~~ Rigidbody of Entity '" + entity_->getName() + "' ~~ \n Mass: " + std::to_string(getMass()) + "\n Rigidbody position: {" +
		std::to_string(getPosition().x) + ", " + std::to_string(getPosition().y) + ", " + std::to_string(getPosition().z) + "\n Transform position: {" +
		std::to_string(getBrotherComponent<Transform>()->getPosition().x) + ", " + std::to_string(getBrotherComponent<Transform>()->getPosition().y) + ", " + std::to_string(getBrotherComponent<Transform>()->getPosition().z) + "\n Mesh AABB: Min{" +
		std::to_string(meshMin.x) + ", " + std::to_string(meshMin.y) + ", " + std::to_string(meshMin.z) + "} Max{" + 
		std::to_string(meshMax.x) + ", " + std::to_string(meshMax.y) + ", " + std::to_string(meshMax.z) + "} \n Linear velocity: {" +
		std::to_string(getLinearVelocity().x) + ", " + std::to_string(getLinearVelocity().y) + ", " + std::to_string(getLinearVelocity().z) + "} \n Angular velocity: {" +
		std::to_string(getAngularVelocity().x) + ", " + std::to_string(getAngularVelocity().y) + ", " + std::to_string(getAngularVelocity().z) + "} \n Force accumulated: {" +
		std::to_string(getTotalForce().x) + ", " + std::to_string(getTotalForce().y) + ", " + std::to_string(getTotalForce().z) + "} \n AABB: Min{" +
		std::to_string(min.x) + ", " + std::to_string(min.y) + ", " + std::to_string(min.z) + "} Max{" +
		std::to_string(max.x) + ", " + std::to_string(max.y) + ", " + std::to_string(max.z) + "} \n Torque accumulated: {" +
		std::to_string(getTotalTorque().x) + ", " + std::to_string(getTotalTorque().y) + ", " + std::to_string(getTotalTorque().z) + "} \n KINEMATIC?: ";
	if (isKinematic())
		s += "YES";
	else
		s += "NO";

	s += "\n STATIC?: ";
	if (isStatic())
		s += "YES";
	else
		s += "NO";
	s += "\n";

	return s;
}
