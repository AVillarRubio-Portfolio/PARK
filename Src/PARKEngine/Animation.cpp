#include "Animation.h"
#include <OgreAnimationState.h>

Animation::Animation() 
{
	
}

Animation::~Animation()
{
	std::cout << "Destructora de Animation" << std::endl;
}

void Animation::update(unsigned int time)
{

}

void Animation::load(json file)
{
	initAnim = file["StartAnim"].get<std::string>();
	loop = file["Loop"];
	enabledInStart = file["EnabledInStart"];

	//currentAnim = Entidad de ogre ->getAnimationState();

	if (enabledInStart)
		enableAnimation();

	else disableAnimation();

}

void Animation::changeAnimation(std::string anim)
{
	//TODO: se necesita coger la entidad de ogre
	//currentAnim = Entidad de ogre ->getAnimationState(anim);

}

void Animation::disableAnimation()
{
	currentAnim->setEnabled(false);
}

void Animation::enableAnimation()
{
	currentAnim->setEnabled(true);
}
