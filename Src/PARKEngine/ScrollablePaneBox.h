#pragma once
#include "Widget.h"
using namespace std;

class ScrollablePaneBox : public Widget
{

public:
	ScrollablePaneBox();
	~ScrollablePaneBox();

	void load(json file);
	virtual void start();
	virtual void receive(Message* msg) {};

	virtual void save(json& file) {};
	virtual std::string getInfo() { return "ScrollablePaneBox"; }

	CEGUI::Window* getScrollablePane() { return scrollablePane_; };

private:
	CEGUI::Window* scrollablePane_;

	vector<string> hijos;
};

REGISTER_TYPE(ScrollablePaneBox)