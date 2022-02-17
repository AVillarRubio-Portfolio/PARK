#pragma once
#include "Component.h"

class BackgroundImage : public Component
{

public:
	BackgroundImage();
	~BackgroundImage();

	virtual void save(json& file) {};

	//Info
	virtual std::string getInfo() { return "BackgroundImage"; }

	void load(json file);

};

REGISTER_TYPE(BackgroundImage)