#include "Widget.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include "SceneManager.h"
#include "Entity.h"

Widget::Widget()
{
}

Widget::~Widget()
{
	//std::cout << "Destructora de Widget" << std::endl;
}

void Widget::load(json file)
{
	std::string name = getEntity()->getName();

	addParameter(type, file["type"]);

	float posX, posY, sizeX, sizeY, offsetX, offsetY;
	posX = posY = sizeX = sizeY = offsetX = offsetY = 0;

	addParameter(posX, file["posX"]);
	addParameter(posY, file["posY"]);
	addParameter(sizeX, file["sizeX"]);
	addParameter(sizeY, file["sizeY"]);
	addParameter(offsetX, file["offsetX"]);
	addParameter(offsetY, file["offsetY"]);

	window = HUDManager::instance()->createWidget(name, type, posX, posY, offsetX, offsetY, sizeX, sizeY);

	setFont(file["font"]);
}

bool Widget::handleEvent(unsigned int time)
{
	return false;
}

void Widget::setText(std::string s)
{
	window->setText(s);
}

void Widget::setFont(std::string s)
{
	window->setFont(s);
}

void Widget::hide()
{
	window->hide();
}

void Widget::show()
{
	window->show();
}

void Widget::setProperty(std::string name, std::string value)
{
	window->setProperty(name, value);
}

Widget * Widget::getChildElement(std::string name)
{
	CEGUI::NamedElement* element =  window->getChildElement(name);
	Entity* e = SceneManager::instance()->currentState()->getEntity(name);
	if (element != nullptr && e->getComponent<Widget>() != nullptr)
		return e->getComponent<Widget>();
	return nullptr;
}