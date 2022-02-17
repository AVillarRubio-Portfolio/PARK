#include "TextBox.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

TextBox::TextBox()
{
	type = "TaharezLook/StaticText";
}

TextBox::~TextBox()
{
	std::cout << "Destructora de TextBox: " << text << std::endl;
}

void TextBox::load(json file)
{
	Widget::load(file);
	staticText_ = static_cast<CEGUI::Window*>(window);

	addParameter(text, file["text"]);

	window->setText(text);
	centreText(file["alignText"]);
	setBackgroundVisible(file["backgroundEnabled"]);
	json color = file["textColor"];
	setColorText(color["topLeft"], color["topRight"], color["botLeft"], color["botRight"]);
}

void TextBox::centreText(std::string s)
{
	window->setProperty("HorzFormatting", s + "Aligned");
}

void TextBox::setBackgroundVisible(string boolean)
{
	window->setProperty("BackgroundEnabled", boolean);
	window->setProperty("FrameEnabled", boolean);
}

void TextBox::setColorText(string topLeft, string topRight, string botLeft, string botRight)
{
	window->setProperty("TextColours", "tl:" + topLeft + " tr:" + topRight + " bl:" + botLeft + " br:" + botRight);
}
