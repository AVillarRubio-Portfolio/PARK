#pragma once
#include "PARKEngine/Component.h"

class NPC;

class NPCManager : public Component
{

public:
	NPCManager();
	~NPCManager();
	
	virtual void load(json file);

	virtual void update(unsigned int time);
	virtual bool handleEvent(unsigned int time);

	virtual void receive(Message* msg);

	virtual std::string getInfo() { return "NPCManager"; };

protected:
	std::vector<NPC*> npcList_;
	int enterTime_;
	int actualTime_;
};
REGISTER_TYPE(NPCManager);