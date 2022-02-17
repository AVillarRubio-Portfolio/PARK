#pragma once
#include "Widget.h"

using namespace std;

class FrameWindowBox : public Widget
{

public:
	FrameWindowBox();
	~FrameWindowBox();


	void load(json file);
	virtual void start();
	virtual void receive(Message* msg) {};

	virtual void save(json& file) {};
	//Info
	virtual std::string getInfo() { return "FrameWindowBox"; }

	virtual std::vector<std::string> getChildren() { return hijos; };


private:
	std::string text;
	vector<string> hijos;

	std::string closeCallback;
	std::string closeCallbackBackParam;

	bool closeWindow(const CEGUI::EventArgs& e);
	bool onClick(const CEGUI::EventArgs & e);
};

REGISTER_TYPE(FrameWindowBox)