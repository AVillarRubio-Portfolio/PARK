#include "Transform.h"
#include "Entity.h"

Transform::Transform():position_(Vector3(0,0,0)), rotation_(Ogre::Quaternion::IDENTITY), scale_(Vector3(1,1,1))
{
	updateAxis();
}

Transform::Transform(Vector3 position, Ogre::Quaternion rotation, Vector3 scale)
	:position_(position), rotation_(rotation), scale_(scale)
{
}

void Transform::load(json file)
{
	json pos = file["position"];
	addParameter(position_.x, pos["x"]);
	addParameter(position_.y, pos["y"]);
	addParameter(position_.z, pos["z"]);

	if (file.contains("scale"))
	{
		json scale = file["scale"];
		addParameter(scale_.x, scale["x"]);
		addParameter(scale_.y, scale["y"]);
		addParameter(scale_.z, scale["z"]);
	}
	updateAxis();
}

void Transform::translate(Vector3 incr, REF_SYSTEM refSys)
{
	switch (refSys)
	{
	case REF_SYSTEM::LOCAL:
		// position is relative to parent so transform downwards
		position_ += rotation_ * incr;
		break;
	case REF_SYSTEM::GLOBAL:
		position_ += incr;
		break;
	}
	notifyChange();
}

void Transform::rotate(Vector3 axis, float degrees, REF_SYSTEM refSys)
{
	//Quaternions are fun
	//Pasamos a radianes
	Ogre::Radian radianAngle = Ogre::Radian(Ogre::Degree(degrees));

	//Obtenemos el cutarnión que simboliza esa transformación
	Ogre::Quaternion q;
	q.FromAngleAxis(radianAngle, axis);

	//Rotamos con ese cuaternión y el sistema dado
	rotate(q, refSys);
}


void Transform::rotate(Quaternion q, REF_SYSTEM refSys)
{
	switch(refSys)
	{
	case REF_SYSTEM::LOCAL:
		//Multiplicamos el cuaternión por la rotación
		rotation_ = rotation_ * q;
		break;
	case REF_SYSTEM::GLOBAL:
		// Rotations are normally relative to local axes, transform up
		rotation_ = rotation_ * rotation_.Inverse() * q * rotation_;
		break;
	default:
		break;
	}
	//Normalizamos el cuaternión
	rotation_.normalise();

	//Update axis
	updateAxis();
}

void Transform::notifyChange()
{
	send(&TransformChanged(TRANSFORM_CHANGED, this));
}

void Transform::scale(Vector3 factor)
{
	scale_ *= factor;
	notifyChange();
}

void Transform::roll(float angle, REF_SYSTEM relativeTo)
{
	rotate(Vector3::UNIT_Z, angle, relativeTo);
}

void Transform::pitch(float angle, REF_SYSTEM relativeTo)
{
	rotate(Vector3::UNIT_X, angle, relativeTo);
}

void Transform::yaw(float angle, REF_SYSTEM relativeTo)
{
	rotate(Vector3::UNIT_Y, angle, relativeTo);

}

void Transform::updateAxis()
{
	forward_ = getRotation() * Vector3::NEGATIVE_UNIT_Z;
	forward_.normalise();

	right_ = getRotation() * Vector3::UNIT_X;
	right_.normalise();

	up_ = getRotation() * Vector3::NEGATIVE_UNIT_Y;
	up_.normalise();

	notifyChange();
}

std::string Transform::getInfo() {
	return "Transform -> More on getDetailedInfo()";
}

std::string Transform::getDetailedInfo()
{
	std::string s = " ~~ Transform of Entity '" + entity_->getName() + "' ~~ \n Position: {" +
		std::to_string(getPosition().x) + ", " + std::to_string(getPosition().y) + ", " + std::to_string(getPosition().z) + "} \n Rotation: {" +
		std::to_string(getRotation().x) + ", " + std::to_string(getRotation().y) + ", " + std::to_string(getRotation().z) + ", " + std::to_string(getRotation().w) + "} \n Scale: {" +
		std::to_string(getScale().x) + ", " + std::to_string(getScale().y) + ", " + std::to_string(getScale().z) + "\n"; 

	return s;
}


Transform::~Transform()
{
	std::cout << "Destructora de Transform" << std::endl;
}

