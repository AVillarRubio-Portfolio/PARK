#ifndef InputManager_H
#define InputManager_H

#include "OISIncludes.h"


class InputManager : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener {
public:
	static void initInstance(std::string inputFile);
	static InputManager* instance();


	virtual ~InputManager();

	void capture();
	bool isKeyDown(std::string accion);

	void addKeyListener(OIS::KeyListener *keyListener, const std::string& instanceName);
	void addMouseListener(OIS::MouseListener *mouseListener, const std::string& instanceName);
	void addJoystickListener(OIS::JoyStickListener *joystickListener, const std::string& instanceName);

	void addMappingValues(std::string file);
	void addKeyBoardMappingValue(std::string name, std::string input);
	void addMouseMappingValue(std::string name, std::string input);

	void removeKeyListener(const std::string& instanceName);
	void removeMouseListener(const std::string& instanceName);
	void removeJoystickListener(const std::string& instanceName);

	void removeKeyListener(OIS::KeyListener *keyListener);
	void removeMouseListener(OIS::MouseListener *mouseListener);
	void removeJoystickListener(OIS::JoyStickListener *joystickListener);

	void removeAllListeners(void);
	void removeAllKeyListeners(void);
	void removeAllMouseListeners(void);
	void removeAllJoystickListeners(void);

	void setWindowExtents(int width, int height);

	OIS::Mouse*    getMouse(void);
	OIS::Keyboard* getKeyboard(void);
	OIS::JoyStick* getJoystick(unsigned int index);

	int getNumOfJoysticks(void);


private:
	InputManager(std::string inputFile);
	InputManager(const InputManager&) { }

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	bool povMoved(const OIS::JoyStickEvent &e, int pov);
	bool axisMoved(const OIS::JoyStickEvent &e, int axis);
	bool sliderMoved(const OIS::JoyStickEvent &e, int sliderID);
	bool buttonPressed(const OIS::JoyStickEvent &e, int button);
	bool buttonReleased(const OIS::JoyStickEvent &e, int button);

	virtual OIS::KeyCode getAsKeyCode(std::string str);
	virtual OIS::MouseButtonID getAsMouseButtonID(std::string str);

	OIS::Mouse        *mMouse;
	OIS::Keyboard     *mKeyboard;
	OIS::InputManager *mInputSystem;

	std::vector<OIS::JoyStick*> mJoysticks;
	std::vector<OIS::JoyStick*>::iterator itJoystick;
	std::vector<OIS::JoyStick*>::iterator itJoystickEnd;

	std::map<std::string, OIS::KeyListener*> mKeyListeners;
	std::map<std::string, OIS::MouseListener*> mMouseListeners;
	std::map<std::string, OIS::JoyStickListener*> mJoystickListeners;

	std::map<std::string, OIS::KeyListener*>::iterator itKeyListener;
	std::map<std::string, OIS::MouseListener*>::iterator itMouseListener;
	std::map<std::string, OIS::JoyStickListener*>::iterator itJoystickListener;

	std::map<std::string, OIS::KeyListener*>::iterator itKeyListenerEnd;
	std::map<std::string, OIS::MouseListener*>::iterator itMouseListenerEnd;
	std::map<std::string, OIS::JoyStickListener*>::iterator itJoystickListenerEnd;

	std::map<std::string, OIS::KeyCode> KeyboardMapping;
	std::map<std::string, OIS::MouseButtonID> MouseMapping;

	static InputManager *instance_;
};
#endif