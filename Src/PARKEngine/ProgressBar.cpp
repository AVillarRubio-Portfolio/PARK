#include "ProgressBar.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

ProgressBar::ProgressBar(): progressBar_(nullptr)
{
	type = "Vanilla/Scrollbar";
}

ProgressBar::~ProgressBar()
{
	std::cout << "Destructora de ProgressBar" << std::endl;
}

void ProgressBar::load(json file)
{
	Widget::load(file);

	progressBar_ = static_cast<CEGUI::ProgressBar*>(window);
	string color = file["BarColor"];
	progressBar_->setProperty("EnabledColour", color);
}

void ProgressBar::setProgress(float progress)
{
	progressBar_->setProgress(progress);
}
