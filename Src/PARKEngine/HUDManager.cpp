#include "HUDManager.h"
#include "SceneManager.h"
#include "OgreManager.h"
#include "ResourceManager.h"
#include "OgreIncludes.h"
#include "OgreManager.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>


using namespace std::placeholders;

std::unique_ptr<HUDManager> HUDManager::instance_;


HUDManager::HUDManager()
{
	//Carga de CEGUI y configurado automatico con elementos de OGRE
	CEGUI::OgreRenderer& myRenderer = CEGUI::OgreRenderer::bootstrapSystem(*static_cast<Ogre::RenderTarget*>(OgreManager::instance()->getWindow()));

	for (std::string s : ResourceManager::instance()->getSchemesPaths())
	{
		CEGUI::SchemeManager::getSingleton().createFromFile(s);
	}

	for (std::string s : ResourceManager::instance()->getFontsPaths())
	{
		CEGUI::FontManager::getSingleton().createFromFile(s);
	}

	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-14");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage("TaharezLook/MouseArrow");
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::LeftButton);

	windowMgr = CEGUI::WindowManager::getSingletonPtr();
}


void HUDManager::initInstance()
{
	//Crea una instancia nueva
	if (instance_.get() == nullptr)
		instance_.reset(new HUDManager());
}

HUDManager* HUDManager::instance()
{
	assert(instance_.get() != nullptr);
	return instance_.get();
}


CEGUI::MouseCursor & HUDManager::getMouseCursor()
{
	return CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
}

void HUDManager::showMouseCursor()
{
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
}

void HUDManager::hideMouseCursor()
{
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
}

bool HUDManager::injectMouseMove(float deltaX, float deltaY)
{
	return CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(deltaX, deltaY);
}

bool HUDManager::injectTimePulse(float timeElapsed)
{
	return CEGUI::System::getSingleton().injectTimePulse(timeElapsed);
}


void HUDManager::addWindow(std::string state)
{
	CEGUI::Window* newWindow = windowMgr->createWindow("DefaultWindow", state);
	windows[state] = newWindow;
}


void HUDManager::changeWindow(std::string state)
{
	auto it = windows.find(state);
	assert(it != windows.end());
		
	setActiveWindow(state);
}

CEGUI::Window * HUDManager::getActiveWindow()
{
	return activeWindow;
}

void HUDManager::setActiveWindow(std::string state)
{
	if (activeWindow != nullptr)
		activeWindow->hide();

	activeWindow = windows[state];
	activeWindow->show();

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(activeWindow);
}


CEGUI::Window* HUDManager::createWidget(std::string name, std::string type, float posX, float posY, float offX, float offY, float tamX, float tamY)
{
	CEGUI::Window* widget = windowMgr->createWindow(type, name);

	widget->setPosition(CEGUI::UVector2(CEGUI::UDim(posX, offX), CEGUI::UDim(posY, offY)));
	widget->setSize(CEGUI::USize(CEGUI::UDim(0, tamX), CEGUI::UDim(0, tamY)));

	activeWindow->addChild(widget);

	return widget;
}


HUDManager::~HUDManager()
{
	std::cout << "Destructora de HUDManager" << std::endl;
}
