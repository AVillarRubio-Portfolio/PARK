#pragma once
#include "Widget.h"

class TextBox : public Widget
{

public:
	TextBox();
	~TextBox();

	void load(json file);

	virtual void receive(Message* msg) {};

	virtual void save(json& file) {};

	virtual std::string getInfo() { return "TextBox"; }

	void centreText(std::string s);
	void setBackgroundVisible(string boolean);
	void setColorText(string topLeft, string topRight, string botLeft, string botRight);

	CEGUI::Window* getStaticText() { return staticText_; };

private:
	std::string text;
	CEGUI::Window* staticText_;
};

REGISTER_TYPE(TextBox)