#include "Button.h"
#include "SceneManager.h"
#include "CallbackManager.h"
#include "InputManager.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

Button::Button() : callbackParam("")
{
	type = "AlfiskoSkin/Button";
}

Button::~Button()
{
	//std::cout << "Destructora de Button" << std::endl;
}

void Button::load(json file)
{
	Widget::load(file);

	std::string text;
	addParameter(text, file["text"]);

	addParameter(callback, file["callback"]);
	addParameter(callbackParam, file["parameter"]);

	pushButton = static_cast<CEGUI::PushButton*>(window);

	pushButton->subscribeEvent(CEGUI::PushButton::EventClicked, &Button::onClick, this);
	pushButton->setText(text);

	//pushButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Button::onClick, this)); // Si el otro fallara, probar con éste
}



bool Button::handleEvent(unsigned int time)
{
	if (InputManager::instance()->isKeyDown("Ray"))
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
	else
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
	}
	
	return false;
}

bool Button::onClick(const CEGUI::EventArgs& e)
{
	CallbackManager::instance()->getCallback(callback)(callbackParam);
	return true;
}
