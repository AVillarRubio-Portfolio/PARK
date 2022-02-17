#include "InputManager.h"
#include "OgreIncludes.h"
#include "ResourceManager.h"
#include <stdio.h>
#include <ctype.h>
#include "Utils.h"
#include "OgreManager.h"

InputManager *InputManager::instance_;

InputManager::InputManager(std::string inputFile) :mMouse(0),mKeyboard(0),mInputSystem(0) 
{
	Ogre::RenderWindow *renderWindow = OgreManager::instance()->getWindow();
	if (!mInputSystem) {
		// Setup basic variables
		OIS::ParamList paramList;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		// Get window handle
		renderWindow->getCustomAttribute("WINDOW", &windowHnd);

		// Fill parameter list
		windowHndStr << (unsigned int)windowHnd;
		paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));


		/*
		// DESCOMENTAR PARA QUITAR EXCLUSIVIDAD DE RATÓN, TECLADO, JOYSTICK A LA VENTANA
		// UNCOMMENT TO SET NON-EXCLUSIVE MODE TO INPUT DEVICES
		//mouse
		paramList.insert(std::make_pair("w32_mouse", "DISCL_FOREGROUND"));
		paramList.insert(std::make_pair("w32_mouse", "DISCL_NONEXCLUSIVE"));

		//keyboard
		paramList.insert(std::make_pair("w32_keyboard", "DISCL_FOREGROUND"));
		paramList.insert(std::make_pair("w32_keyboard", "DISCL_NONEXCLUSIVE"));
		paramList.insert(std::make_pair("w32_keyboard", "DISCL_NOWINKEY"));

		//joystick/gamepad
		paramList.insert(std::make_pair("w32_joystick", "DISCL_FOREGROUND"));
		paramList.insert(std::make_pair("w32_joystick", "DISCL_NONEXCLUSIVE"));
		*/

		// Create inputsystem
		mInputSystem = OIS::InputManager::createInputSystem(paramList);

		// If possible create a buffered keyboard
		// (note: if below line doesn't compile, try:  if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0) {
		//if( mInputSystem->numKeyboards() > 0 ) {
		if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0) {
			mKeyboard = static_cast<OIS::Keyboard*>(mInputSystem->createInputObject(OIS::OISKeyboard, true));
			mKeyboard->setEventCallback(this);
		}

		// If possible create a buffered mouse
		// (note: if below line doesn't compile, try:  if (mInputSystem->getNumberOfDevices(OIS::OISMouse) > 0) {
		//if( mInputSystem->numMice() > 0 ) {
		if (mInputSystem->getNumberOfDevices(OIS::OISMouse) > 0) {
			mMouse = static_cast<OIS::Mouse*>(mInputSystem->createInputObject(OIS::OISMouse, true));
			mMouse->setEventCallback(this);

			// Get window size
			unsigned int width, height, depth;
			int left, top;
			renderWindow->getMetrics(width, height, depth, left, top);

			// Set mouse region
			this->setWindowExtents(width, height);
		}

		// If possible create all joysticks in buffered mode
		// (note: if below line doesn't compile, try:  if (mInputSystem->getNumberOfDevices(OIS::OISJoyStick) > 0) {
		//if( mInputSystem->numJoySticks() > 0 ) {
		if (mInputSystem->getNumberOfDevices(OIS::OISJoyStick) > 0) {
			//mJoysticks.resize( mInputSystem->numJoySticks() );
			mJoysticks.resize(mInputSystem->getNumberOfDevices(OIS::OISJoyStick));

			itJoystick = mJoysticks.begin();
			itJoystickEnd = mJoysticks.end();
			for (; itJoystick != itJoystickEnd; ++itJoystick) {
				(*itJoystick) = static_cast<OIS::JoyStick*>(mInputSystem->createInputObject(OIS::OISJoyStick, true));
				(*itJoystick)->setEventCallback(this);
			}
		}
	}
	addMappingValues(inputFile);
}

void InputManager::initInstance(std::string inputFile)
{
	if (instance_ == nullptr)
		instance_ = new InputManager(inputFile);	
}

InputManager* InputManager::instance() 
{
	assert(instance_ != nullptr);
	return instance_;
}

InputManager::~InputManager() {
	std::cout << "Destructora de InputManager" << std::endl;
	if (mInputSystem) {
		if (mMouse) {
			mInputSystem->destroyInputObject(mMouse);
			mMouse = 0;
		}
		if (mKeyboard) {
			mInputSystem->destroyInputObject(mKeyboard);
			mKeyboard = 0;
		}

		if (mJoysticks.size() > 0) {
			itJoystick = mJoysticks.begin();
			itJoystickEnd = mJoysticks.end();
			for (; itJoystick != itJoystickEnd; ++itJoystick) {
				mInputSystem->destroyInputObject(*itJoystick);
			}

			mJoysticks.clear();
		}

		// If you use OIS1.0RC1 or above, uncomment this line
		// and comment the line below it
		mInputSystem->destroyInputSystem(mInputSystem);
		//mInputSystem->destroyInputSystem();
		mInputSystem = 0;

		// Clear Listeners
		mKeyListeners.clear();
		mMouseListeners.clear();
		mJoystickListeners.clear();
	}
}

void InputManager::capture() {
	// Need to capture / update each device every frame
	if (mMouse) {
		mMouse->capture();
	}
	mMouse->getMouseState();
	if (mKeyboard) {
		mKeyboard->capture();
	}

	if (mJoysticks.size() > 0) {
		itJoystick = mJoysticks.begin();
		itJoystickEnd = mJoysticks.end();
		for (; itJoystick != itJoystickEnd; ++itJoystick) {
			(*itJoystick)->capture();
		}
	}
}

bool InputManager::isKeyDown(std::string accion)
{
	auto itK = KeyboardMapping.find(accion);
	if (itK != KeyboardMapping.end())
		return mKeyboard->isKeyDown(itK->second);
	auto itM = MouseMapping.find(accion);
	if (itM != MouseMapping.end())
		return mMouse->getMouseState().buttonDown(itM->second);

	return false;
}

void InputManager::addKeyListener(OIS::KeyListener *keyListener, const std::string& instanceName) {
	if (mKeyboard) {
		// Check for duplicate items
		itKeyListener = mKeyListeners.find(instanceName);
		if (itKeyListener == mKeyListeners.end()) {
			mKeyListeners[instanceName] = keyListener;
		}
		else {
			// Duplicate Item
		}
	}
}

void InputManager::addMouseListener(OIS::MouseListener *mouseListener, const std::string& instanceName) {
	if (mMouse) {
		// Check for duplicate items
		itMouseListener = mMouseListeners.find(instanceName);
		if (itMouseListener == mMouseListeners.end()) {
			mMouseListeners[instanceName] = mouseListener;
		}
		else {
			// Duplicate Item
		}
	}
}

void InputManager::addJoystickListener(OIS::JoyStickListener *joystickListener, const std::string& instanceName) {
	if (mJoysticks.size() > 0) {
		// Check for duplicate items
		itJoystickListener = mJoystickListeners.find(instanceName);
		if (itJoystickListener == mJoystickListeners.end()) {
			mJoystickListeners[instanceName] = joystickListener;
		}
		else {
			// Duplicate Item
		}
	}
}

void InputManager::addMappingValues(std::string file)
{
	mKeyboard->setTextTranslation(mKeyboard->Ascii);
	json js = ResourceManager::instance()->getJsonByKey(file);
	for (json Jkeyboard : js["Keyboard"]) {
		KeyboardMapping.insert(std::pair<std::string, OIS::KeyCode>(Jkeyboard["Id"], getAsKeyCode(Jkeyboard["key"])));
	}
	for (json JMouse : js["Mouse"]) {
		MouseMapping.insert(std::pair<std::string, OIS::MouseButtonID>(JMouse["Id"], getAsMouseButtonID(JMouse["key"])));
	}
}

void InputManager::addKeyBoardMappingValue(std::string name, std::string input)
{
	KeyboardMapping.insert(std::pair<std::string, OIS::KeyCode>(name, mKeyboard->getAsKeyCode(input)));
}

void InputManager::addMouseMappingValue(std::string name, std::string input)
{
	MouseMapping.insert(std::pair<std::string, OIS::MouseButtonID>(name, getAsMouseButtonID(input)));
}

void InputManager::removeKeyListener(const std::string& instanceName) {
	// Check if item exists
	itKeyListener = mKeyListeners.find(instanceName);
	if (itKeyListener != mKeyListeners.end()) {
		mKeyListeners.erase(itKeyListener);
	}
	else {
		// Doesn't Exist
	}
}

void InputManager::removeMouseListener(const std::string& instanceName) {
	// Check if item exists
	itMouseListener = mMouseListeners.find(instanceName);
	if (itMouseListener != mMouseListeners.end()) {
		mMouseListeners.erase(itMouseListener);
	}
	else {
		// Doesn't Exist
	}
}

void InputManager::removeJoystickListener(const std::string& instanceName) {
	// Check if item exists
	itJoystickListener = mJoystickListeners.find(instanceName);
	if (itJoystickListener != mJoystickListeners.end()) {
		mJoystickListeners.erase(itJoystickListener);
	}
	else {
		// Doesn't Exist
	}
}

void InputManager::removeKeyListener(OIS::KeyListener *keyListener) {
	itKeyListener = mKeyListeners.begin();
	itKeyListenerEnd = mKeyListeners.end();
	for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) {
		if (itKeyListener->second == keyListener) {
			mKeyListeners.erase(itKeyListener);
			break;
		}
	}
}

void InputManager::removeMouseListener(OIS::MouseListener *mouseListener) {
	itMouseListener = mMouseListeners.begin();
	itMouseListenerEnd = mMouseListeners.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener) {
		if (itMouseListener->second == mouseListener) {
			mMouseListeners.erase(itMouseListener);
			break;
		}
	}
}

void InputManager::removeJoystickListener(OIS::JoyStickListener *joystickListener) {
	itJoystickListener = mJoystickListeners.begin();
	itJoystickListenerEnd = mJoystickListeners.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener) {
		if (itJoystickListener->second == joystickListener) {
			mJoystickListeners.erase(itJoystickListener);
			break;
		}
	}
}

void InputManager::removeAllListeners(void) {
	mKeyListeners.clear();
	mMouseListeners.clear();
	mJoystickListeners.clear();
}

void InputManager::removeAllKeyListeners(void) {
	mKeyListeners.clear();
}

void InputManager::removeAllMouseListeners(void) {
	mMouseListeners.clear();
}

void InputManager::removeAllJoystickListeners(void) {
	mJoystickListeners.clear();
}

void InputManager::setWindowExtents(int width, int height) {
	// Set mouse region (if window resizes, we should alter this to reflect as well)
	const OIS::MouseState &mouseState = mMouse->getMouseState();
	mouseState.width = width;
	mouseState.height = height;
}

OIS::Mouse* InputManager::getMouse(void) {
	return mMouse;
}

OIS::Keyboard* InputManager::getKeyboard(void) {
	return mKeyboard;
}

OIS::JoyStick* InputManager::getJoystick(unsigned int index) {
	// Make sure it's a valid index
	if (index < mJoysticks.size()) {
		return mJoysticks[index];
	}

	return 0;
}

int InputManager::getNumOfJoysticks(void) {
	// Cast to keep compiler happy ^^
	return (int)mJoysticks.size();
}

bool InputManager::keyPressed(const OIS::KeyEvent &e) {
	itKeyListener = mKeyListeners.begin();
	itKeyListenerEnd = mKeyListeners.end();
	for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) {
		if (!itKeyListener->second->keyPressed(e))
			break;
	}

	return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent &e) {
	itKeyListener = mKeyListeners.begin();
	itKeyListenerEnd = mKeyListeners.end();
	for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) {
		if (!itKeyListener->second->keyReleased(e))
			break;
	}

	return true;
}


bool InputManager::mouseMoved(const OIS::MouseEvent &e) {
	itMouseListener = mMouseListeners.begin();
	itMouseListenerEnd = mMouseListeners.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener) {
		if (!itMouseListener->second->mouseMoved(e))
			break;
	}

	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	itMouseListener = mMouseListeners.begin();
	itMouseListenerEnd = mMouseListeners.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener) {
		if (!itMouseListener->second->mousePressed(e, id))
			break;
	}

	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	itMouseListener = mMouseListeners.begin();
	itMouseListenerEnd = mMouseListeners.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener) {
		if (!itMouseListener->second->mouseReleased(e, id))
			break;
	}

	return true;
}

bool InputManager::povMoved(const OIS::JoyStickEvent &e, int pov) {
	itJoystickListener = mJoystickListeners.begin();
	itJoystickListenerEnd = mJoystickListeners.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener) {
		if (!itJoystickListener->second->povMoved(e, pov))
			break;
	}

	return true;
}

bool InputManager::axisMoved(const OIS::JoyStickEvent &e, int axis) {
	itJoystickListener = mJoystickListeners.begin();
	itJoystickListenerEnd = mJoystickListeners.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener) {
		if (!itJoystickListener->second->axisMoved(e, axis))
			break;
	}

	return true;
}

bool InputManager::sliderMoved(const OIS::JoyStickEvent &e, int sliderID) {
	itJoystickListener = mJoystickListeners.begin();
	itJoystickListenerEnd = mJoystickListeners.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener) {
		if (!itJoystickListener->second->sliderMoved(e, sliderID))
			break;
	}

	return true;
}

bool InputManager::buttonPressed(const OIS::JoyStickEvent &e, int button) {
	itJoystickListener = mJoystickListeners.begin();
	itJoystickListenerEnd = mJoystickListeners.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener) {
		if (!itJoystickListener->second->buttonPressed(e, button))
			break;
	}

	return true;
}

bool InputManager::buttonReleased(const OIS::JoyStickEvent &e, int button) {
	itJoystickListener = mJoystickListeners.begin();
	itJoystickListenerEnd = mJoystickListeners.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener) {
		if (!itJoystickListener->second->buttonReleased(e, button))
			break;
	}

	return true;
}




OIS::KeyCode InputManager::getAsKeyCode(std::string str)
{

	for (int i = 0; i < str.size(); i++) { str.at(i) = toupper(str.at(i)); }
	if (str == "A") return OIS::KC_A;
	else if (str == "B") return OIS::KC_B;
	else if (str == "C") return OIS::KC_C;
	else if (str == "D") return OIS::KC_D;
	else if (str == "E") return OIS::KC_E;
	else if (str == "F") return OIS::KC_F;
	else if (str == "G") return OIS::KC_G;
	else if (str == "H") return OIS::KC_H;
	else if (str == "I") return OIS::KC_I;
	else if (str == "J") return OIS::KC_J;
	else if (str == "K") return OIS::KC_K;
	else if (str == "L") return OIS::KC_L;
	else if (str == "M") return OIS::KC_M;
	else if (str == "N") return OIS::KC_N;
	else if (str == "O") return OIS::KC_O;
	else if (str == "P") return OIS::KC_P;
	else if (str == "Q") return OIS::KC_Q;
	else if (str == "R") return OIS::KC_R;
	else if (str == "S") return OIS::KC_S;
	else if (str == "T") return OIS::KC_T;
	else if (str == "U") return OIS::KC_U;
	else if (str == "V") return OIS::KC_V;
	else if (str == "W") return OIS::KC_W;
	else if (str == "X") return OIS::KC_X;
	else if (str == "Y") return OIS::KC_Y;
	else if (str == "Z") return OIS::KC_Z;
	else if (str == "ENTER") return OIS::KC_RETURN;
	else if (str == "ESC") return OIS::KC_ESCAPE;
	else if (str == "SPACE") return OIS::KC_SPACE;
	else if (str == "LEFTCTRL") return OIS::KC_LCONTROL;
	else if (str == "RIGHTCTRL") return OIS::KC_RCONTROL;
	else if (str == "LEFTSHIFT") return OIS::KC_LSHIFT;
	else if (str == "RIGHTSHIFT") return OIS::KC_RSHIFT;
	else if (str == "TAB") return OIS::KC_TAB;
	else if (str == "0") return OIS::KC_0;
	else if (str == "1") return OIS::KC_1;
	else if (str == "2") return OIS::KC_2;
	else if (str == "3") return OIS::KC_3;
	else if (str == "4") return OIS::KC_4;
	else if (str == "5") return OIS::KC_5;
	else if (str == "6") return OIS::KC_6;
	else if (str == "7") return OIS::KC_7;
	else if (str == "8") return OIS::KC_8;
	else if (str == "9") return OIS::KC_9;
	else if (str == "INS") return OIS::KC_INSERT;
	else if (str == "SUPR") return OIS::KC_SUBTRACT;
	else return OIS::KC_UNASSIGNED;
}

OIS::MouseButtonID InputManager::getAsMouseButtonID(std::string str)
{
	if (str == "MOUSE_LEFT") return OIS::MB_Left;
	else if (str == "MOUSE_RIGHT") return OIS::MB_Right;
	else if (str == "MOUSE_MIDDLE") return OIS::MB_Middle;
	else if (str == "MOUSE_BUTTON3") return OIS::MB_Button3;
	else if (str == "MOUSE_BUTTON4") return OIS::MB_Button4;
	else if (str == "MOUSE_BUTTON5") return OIS::MB_Button5;
	else if (str == "MOUSE_BUTTON6") return OIS::MB_Button6;
	else if (str == "MOUSE_BUTTON7") return OIS::MB_Button7;
	else return OIS::MB_Unassigned;
}
