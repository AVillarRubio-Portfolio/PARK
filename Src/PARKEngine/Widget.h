#pragma once
#include "Component.h"
#include "HUDManager.h"

using namespace std;

class Widget : public Component
{
protected:

	CEGUI::Window* window;
	std::string type;

public:
	Widget();
	~Widget();

	virtual void load(json file);
	virtual std::string getInfo() { return "widget"; };

	//Métodos principales
	virtual void render(unsigned int time) {}
	virtual void update(unsigned int time) {}
	virtual bool handleEvent(unsigned int time);

	virtual void receive(Message* msg) {};

	virtual void save(json& file) {};


	//Wrappers
	void setText(std::string s);
	void setFont(std::string s);

	virtual void hide();
	virtual void show();

	virtual void setProperty(std::string name, std::string value);

	virtual Widget* getChildElement(std::string name);
};

REGISTER_TYPE(Widget)