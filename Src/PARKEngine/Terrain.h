#pragma once
#include "Component.h"
class TerrainGenerator;

class Terrain : public Component
{
public:
	Terrain();
	Terrain(std::string terrainFile);
	~Terrain();

	virtual void load(json file);
	std::string getInfo() { return "Terrain"; }

	virtual void save(json& file) {};

	TerrainGenerator* terrainCreator_;
};

REGISTER_TYPE(Terrain)