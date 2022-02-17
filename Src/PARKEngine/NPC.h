#pragma once
#include "Component.h"

class NPC : public Component
{
public:
	NPC();
	~NPC();

	void load(json file);
	virtual void render(unsigned int time) {};
	virtual void update(unsigned int time) {};
	virtual bool handleEvent(SDL_Event* e, unsigned int time) { return false; };
	virtual void receive(Message* msg) {};

private:
	int hunger;
	int thirst;
};

REGISTER_TYPE(NPC)