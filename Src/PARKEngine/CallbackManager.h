#pragma once
#include <memory>
#include <map>
#include <functional>

class CallbackManager
{
public:
	static void initInstance();
	static CallbackManager* instance();

	template<typename T>
	void addCallback(std::string keyName, bool(T::* cb)(std::string), T* obj);

	std::function<void(std::string)> getCallback(std::string name);

	~CallbackManager();

private:
	static std::unique_ptr<CallbackManager> instance_;

	std::map <std::string, std::function<bool(std::string)>> callbacks;

	void initCallbacks();

	CallbackManager();

	bool onExit(std::string null);
	bool onChangeState(std::string state);
	bool EffectVolumeChange(std::string vol);
	bool MusicVolumeChange(std::string vol);

	// Callbacks para la construccion
	bool construct(std::string buildName);
	bool setDeleteBuildingActive(std::string s);

	// Callbacks para activar y desactivar paneles
	bool setToolsPanelActive(std::string boolean);
	bool setConstructPanelActive(std::string boolean);
	bool setRoadConstructionActive(std::string boolean);
	bool setAmusementsConstructionActive(std::string boolean);
	bool setRestaurantsConstructionActive(std::string boolean);
	bool setToiletsConstructionActive(std::string boolean);
	bool setInfoPanelActive(std::string boolean);
	bool setNPCInfoPanelActive(std::string boolean);
};
