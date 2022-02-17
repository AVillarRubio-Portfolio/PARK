#pragma once
#include "Component.h"


enum REF_SYSTEM { LOCAL, GLOBAL };

class Transform : public Component
{
public:

	Transform();
	Transform(Vector3 position, Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY, Vector3 scale = Vector3(1,1,1));

	~Transform();

	virtual void load(json file);

	//GETTERS
	//Position getter
	Vector3 getPosition() { return position_; };
	//Rotation getter
	Ogre::Quaternion getRotation() { return rotation_; };
	//Scale getter
	Vector3 getScale() { return scale_; };


	//SETTERS
	//Position setter
	void setPosition(Vector3 pos) { this->position_ = pos; notifyChange();};
	//Rotation setter
	void setRotation(Ogre::Quaternion rotation) { this->rotation_ = rotation; updateAxis(); notifyChange();};
	//Scale setter
	void setScale(Vector3 scale) { this->scale_ = scale; notifyChange();};

	//TRANSFORMATIONS
	//Translate the component given a certain increment
	void translate(Vector3 incr, REF_SYSTEM refSys = REF_SYSTEM::GLOBAL);
	//Rotate the component around a certain axis
	void rotate(Vector3 axis, float degrees, REF_SYSTEM refSys = REF_SYSTEM::LOCAL);
	//Multiplies the component scale for a given factor
	void scale(Vector3 factor);

	//SPECIFIC ROTATIONS
	//Rotates around Z axis
	void roll(float angle, REF_SYSTEM relativeTo = REF_SYSTEM::LOCAL);
	//Rotates around X axis
	void pitch(float angle, REF_SYSTEM relativeTo = REF_SYSTEM::LOCAL);
	//Rotates around Y axis
	void yaw(float angle, REF_SYSTEM relativeTo = REF_SYSTEM::LOCAL);

	//LOCAL AXIS
	//Returns forward vector
	Vector3 forward() { return forward_; };
	//Returns right vector
	Vector3 right() { return right_; };
	//Returns up vector
	Vector3 up() { return up_; };


	//Debug information
	virtual std::string getInfo();
	virtual std::string getDetailedInfo();

	virtual void save(json& file) {};


protected:
	//Position
	Vector3 position_;
	//Orientation (x,y,z,w)
	Ogre::Quaternion rotation_;
	//Scale
	Vector3 scale_;

	//LOCAL AXIS
	//Forward
	Vector3 forward_;
	//Right
	Vector3 right_;
	//Up
	Vector3 up_;

	//Rotate wthin a reference system
	void rotate(Quaternion q,  REF_SYSTEM refSys = REF_SYSTEM::LOCAL);
	//Updates axis
	void updateAxis();
	//Avisa a los componentes de la misma entidad de que han cambiado sus atributos
	void notifyChange();



};

REGISTER_TYPE(Transform)