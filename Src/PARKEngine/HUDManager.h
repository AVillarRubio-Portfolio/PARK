#pragma once
#include "Utils.h"

class OgreManager;

//Espacio de nombres de CEGUI (predeclaraciones)
namespace CEGUI
{
	class WindowManager;
	class Window;
	class MouseCursor;
	class PushButton;
	class EventArgs;
	class ProgressBar;
	class PushButton;
}

class HUDManager
{
public:	
	static void initInstance();

	static HUDManager* instance();


	void addWindow(std::string state);
	void changeWindow(std::string state);

	CEGUI::Window* getActiveWindow();
	void setActiveWindow(std::string state);

	//WRAPPERS
	//Enseña/oculta cursor
	void showMouseCursor();
	void hideMouseCursor();

	//Mueve el ratón
	bool injectMouseMove(float deltaX, float deltaY);

	bool injectTimePulse(float timeElapsed);

	
	CEGUI::Window* createWidget(std::string name, std::string type, float posX, float posY, float offX, float offY, float tamX, float tamY);


	~HUDManager();
private:
	friend class OgreManager;

	static std::unique_ptr<HUDManager> instance_;

	std::map<std::string, CEGUI::Window*> windows;
	CEGUI::WindowManager* windowMgr;
	CEGUI::Window* activeWindow;


	HUDManager();

	//Returns mouse cursor
	CEGUI::MouseCursor& getMouseCursor();
};
