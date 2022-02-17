#include "ScrollablePaneBox.h"
#include "SceneManager.h"
#include "Button.h"
#include "TextBox.h"
#include "ProgressBar.h"
#include "ImageBox.h"
#include "CallbackManager.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include "Entity.h"

ScrollablePaneBox::ScrollablePaneBox()
{
	type = "TaharezLook/StaticText";
}

ScrollablePaneBox::~ScrollablePaneBox()
{
	std::cout << "Destructora de ScrollablePaneBox" << std::endl;
}

void ScrollablePaneBox::load(json file)
{
	Widget::load(file);
	scrollablePane_ = static_cast<CEGUI::Window*>(window);

	//Para los hijos del panel
	for (json h : file["Hijos"]) {
		hijos.push_back(h["name"]);
	}
}

void ScrollablePaneBox::start()
{
	for (string s : hijos) {
		Entity* e = SceneManager::instance()->currentState()->getEntity(s);
		if (e->getComponent<Button>() != nullptr)
			window->addChild(SceneManager::instance()->currentState()->getEntity(s)->getComponent<Button>()->getPushButton());
		else if (e->getComponent<TextBox>() != nullptr)
			window->addChild(SceneManager::instance()->currentState()->getEntity(s)->getComponent<TextBox>()->getStaticText());
		else if (e->getComponent<ProgressBar>() != nullptr)
			window->addChild(SceneManager::instance()->currentState()->getEntity(s)->getComponent<ProgressBar>()->getProgressBar());
		else if (e->getComponent<ImageBox>() != nullptr)
			window->addChild(SceneManager::instance()->currentState()->getEntity(s)->getComponent<ImageBox>()->getStaticImage());
	}
}