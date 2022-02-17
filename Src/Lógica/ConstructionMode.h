#pragma once
#include "PARKEngine/Component.h"
#include <vector>
#include <algorithm>

using namespace std;

class Matrix;
class Edificio;
class BureauCrazyManager;

class ConstructionMode : public Component
{
public:
	ConstructionMode();
	~ConstructionMode();

	virtual void load(json file);

	virtual void start();

	virtual void render(unsigned int time) {};
	virtual void update(unsigned int time);
	virtual bool handleEvent(unsigned int time);
	virtual void receive(Message* msg) {};

	virtual void buildInMatrix(int i, int j, std::string name);
	void construct(string bName);
	void deactivateThisConstruction();

	bool getDeleteActice() { return deleteActive_; };
	void setDeleteActive(bool b) { deleteActive_ = b; };
	
	virtual std::string getInfo() { return "ConstructionMode"; };

private:
	bool constructActive_;
	bool canConst_;

	BureauCrazyManager* bureauCrazyManager_;

	Entity* matrixEntity_;
	Entity* nodeEntity_;

	Entity* buildingEntity_;
	Edificio* build_;

	list<Entity*> nodes_;
	vector<string> buildingTypes_;

	list<Entity*> getNodesToConstruct(Entity* node, Ogre::Vector3 mousePos);
	bool canConstruct(int n);

	Ogre::Vector3 getPosToConstruct();
	void setNodesType();
	void setBuilding();
	void createEntryExitRoad(string roadName);
	void setEntryExit();
	void setNodeMaterial(bool enable, bool can);

	//Para eliminar edificios
	bool deleteActive_;
	void deleteBuilding();
};

REGISTER_TYPE(ConstructionMode)