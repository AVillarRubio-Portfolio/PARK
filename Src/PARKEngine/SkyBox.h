#pragma once
#include "Component.h"

class SkyBox : public Component
{

public:
	SkyBox();
	~SkyBox();

	virtual void save(json& file) {};

	void load(json file);

private:
	Ogre::Plane* plane;
};

REGISTER_TYPE(SkyBox)