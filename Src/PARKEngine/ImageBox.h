#pragma once
#include "Widget.h"

class ImageBox : public Widget
{

public:
	ImageBox();
	~ImageBox();

	void load(json file);

	virtual void receive(Message* msg) {};

	virtual void save(json& file) {};
	//Info
	virtual std::string getInfo() { return "ImageBox"; }

	CEGUI::Window* getStaticImage() { return staticImage_; };


private:
	std::string text;
	CEGUI::Window* staticImage_;

		void setBackgroundVisible(string boolean);
};

REGISTER_TYPE(ImageBox)