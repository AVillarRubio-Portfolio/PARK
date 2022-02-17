#pragma once
#include <PARKEngine/Component.h>

class Reproductor : public Component
{
public:
	Reproductor();
	~Reproductor();

	virtual void render(unsigned int time) {};
	virtual void update(unsigned int time) {};
	virtual bool handleEvent(unsigned int time) { return false; };

	virtual void receive(Message* msg);

	virtual void load(json file) {};
	virtual std::string getInfo() { return "Reproductor"; };
};

REGISTER_TYPE(Reproductor)

