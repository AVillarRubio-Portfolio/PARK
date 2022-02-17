#include "Component.h"
#include "ResourceManager.h"
#include "Entity.h"

Component::Component() : active_(true), entity_(nullptr)
{
}



void Component::releaseEntity()
{
	delete entity_;  
	entity_ = nullptr;
}


Component::~Component()
{
	//std::cout << "Destructora de Component" << std::endl;
}
