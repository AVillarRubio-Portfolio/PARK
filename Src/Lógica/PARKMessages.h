#pragma once
#include "PARKEngine/Messages.h"

//Predeclaraciones
class NPC;
class Edificio;

struct MessageInfo : public Message {
	MessageInfo(MessageId mType, Entity* mEntity, header_t_ size = sizeof(Message)) :
		mEntity_(mEntity), Message(mType, size) {
	}
	Entity* mEntity_;
};

struct IsBuildingUnlocked : public Message {
	IsBuildingUnlocked(MessageId mType, Edificio* edificio, header_t_ size = sizeof(Message)) :
		 Message(mType, size) , edificio_(edificio){
	}
	Edificio* edificio_;
};

struct BuildingNotUnlocked : public Message {
	BuildingNotUnlocked(MessageId mType, Edificio* edificio, header_t_ size = sizeof(Message)) :
		Message(mType, size), edificio_(edificio) {
	}
	Edificio* edificio_;
};

struct BuildingUnlocked : public Message {
	BuildingUnlocked(MessageId mType, Edificio* edificio, header_t_ size = sizeof(Message)) :
		Message(mType, size), edificio_(edificio) {
	}
	Edificio* edificio_;
};

//Se ha seleccionado a cierto NPC
struct NPCSelected : public Message {
	NPCSelected(MessageId mType, NPC* selected, header_t_ size = sizeof(Message)) :
		Message(mType, size), selected_(selected) {
	}
	NPC* selected_;
};

//Un NPC se va del parque
struct NPCOut : public Message {
	NPCOut(MessageId mType, NPC* npc, header_t_ size = sizeof(Message)) :
		Message(mType, size), npc_(npc) {
	}
	NPC* npc_;
};

//Cierto NPC ha entrado en cierta atracción
struct NPCEnteredAttraction : public Message {
	NPCEnteredAttraction(MessageId mType, NPC* npc, Edificio* attraction, header_t_ size = sizeof(Message)) :
		Message(mType, size), npc_(npc), attraction_(attraction) {
	}
	NPC* npc_;
	Edificio* attraction_;
};
