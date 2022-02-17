#include "Reproductor.h"

#include <PARKEngine/Entity.h>
#include <PARKEngine/Transform.h>
#include <PARKEngine/AudioManager.h>
#include "PARKMessages.h"


Reproductor::Reproductor()
{
}


Reproductor::~Reproductor()
{
	std::cout << "Destructora de Reproductor" << std::endl;
}

void Reproductor::receive(Message * msg)
{
	//SOLO FALTARIA CREAR SONIDOS Y SUS RESPECTIVAS RESPUESTAS
	switch (msg->mType_)
	{
	case CREATED_BUILDING:
	{
		MessageInfo* inf = static_cast<MessageInfo*>(msg);
		AudioManager::instance()->PLAY_3D_SOUND("build3d", inf->mEntity_->getComponent<Transform>()->getPosition());
	}
	case GAME_START:
	{
		AudioManager::instance()->PLAY_2D_SOUND("Start");
	}
	case NPC_IN:
	{
		AudioManager::instance()->PLAY_2D_SOUND("NPCIn");
	}
	default:
		break;
	};
}
