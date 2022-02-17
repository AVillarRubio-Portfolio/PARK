#include "Terrain.h"
#include <iostream>
#include "OgreManager.h"

#include "ResourceManager.h"

Terrain::Terrain():terrainCreator_(nullptr)
{
}

Terrain::Terrain(std::string terrainFile)
{
	terrainCreator_ = OgreManager::instance()->createTerrain(terrainFile);
}

void Terrain::load(json file)
{
	terrainCreator_ = OgreManager::instance()->createTerrain(file["file"]);
}

Terrain::~Terrain()
{
	std::cout << "Destructora de Terrain" << std::endl;
	delete terrainCreator_;
}