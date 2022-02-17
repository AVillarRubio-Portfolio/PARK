#include "SkyBox.h"
#include "OgreManager.h"

SkyBox::SkyBox():plane(nullptr)
{

}

SkyBox::~SkyBox()
{
	delete plane;
}

void SkyBox::load(json file)
{
	 plane = OgreManager::instance()->createSkyBox();
}
