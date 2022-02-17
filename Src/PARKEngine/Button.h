#pragma once
#include "Widget.h"

class Button : public Widget
{
private:
	std::string callback;
	std::string callbackParam;

public:
	Button();
	~Button();

	void load(json file);

	//Métodos principales
	virtual void render(unsigned int time) {}
	virtual void update(unsigned int time) {}
	virtual bool handleEvent(unsigned int time);

	virtual bool onClick(const CEGUI::EventArgs& e);

	virtual void receive(Message* msg) {};

	virtual void save(json& file) {};

	//Info
	virtual std::string getInfo() { return "Button"; }

	//Getter
	std::string getCallback() { return callback; };
	CEGUI::PushButton* getPushButton() { return pushButton; };

private:
	CEGUI::PushButton* pushButton;
};

REGISTER_TYPE(Button)