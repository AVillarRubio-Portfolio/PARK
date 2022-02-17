#pragma once
#include <string>

class OgreManager;
class SceneManager;
class AudioManager;
class PhysicsManager;
class InputManager;
class HUDManager;


class Game {
public:
	Game(std::string basicConfig);
	~Game();

	void start();
	void run();

private:
	PhysicsManager* physicsManager_;
	SceneManager* sceneManager_;
	OgreManager* ogreManager_;
	AudioManager* audioManager_;
	InputManager* inputManager_;
	HUDManager* hudManager_;

	bool exit;
};