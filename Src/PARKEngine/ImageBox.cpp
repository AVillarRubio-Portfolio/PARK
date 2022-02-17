#include "ImageBox.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

ImageBox::ImageBox()
{
	type = "TaharezLook/StaticImage";
}

ImageBox::~ImageBox()
{
	std::cout << "Destructora de ImageBox" << std::endl;
}

void ImageBox::load(json file)
{
	Widget::load(file);
	staticImage_ = static_cast<CEGUI::Window*>(window);

	std::string nombre;
	nombre = file["imageName"].get<std::string>();

	if (!CEGUI::ImageManager::getSingleton().isDefined(nombre)){
		CEGUI::ImageManager::getSingleton().addFromImageFile(nombre, nombre);
	}
		
	staticImage_->setProperty("Image", nombre);
	setBackgroundVisible("False");
}

void ImageBox::setBackgroundVisible(string boolean)
{
	window->setProperty("BackgroundEnabled", boolean);
	window->setProperty("FrameEnabled", boolean);
}
