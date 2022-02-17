#include "TerrainCreator.h"
#include "Utils.h"

TerrainGenerator::TerrainGenerator() : scnMgn_(nullptr), terrainJson_(NULL)
{

}

TerrainGenerator::TerrainGenerator(Ogre::SceneManager * scnMgn, Ogre::Light * light, std::string terrainFile)
	: scnMgn_(scnMgn)
{
	terrainJson_ = ResourceManager::instance()->getJsonByKey(terrainFile);

	mTerrainGlobals_ = OGRE_NEW Ogre::TerrainGlobalOptions();

	mTerrainGroup_ = OGRE_NEW Ogre::TerrainGroup(scnMgn, Ogre::Terrain::ALIGN_X_Z,
		terrainJson_["maps"][0]["terrainSize"], terrainJson_["maps"][0]["worldSize"]);

	mTerrainGroup_->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
	mTerrainGroup_->setOrigin(Vector3::ZERO);

	configureTerrainDefaults(light);

	for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)
			defineTerrain(x, y);

	mTerrainGroup_->loadAllTerrains(true);

	if (mTerrainsImported_)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup_->getTerrainIterator();

		while (ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}

	mTerrainGroup_->freeTemporaryResources();
}


TerrainGenerator::~TerrainGenerator()
{
	std::cout << "Destructora de TerrainGenerator" << std::endl;
	OGRE_DELETE mTerrainGroup_;
	OGRE_DELETE	mTerrainGlobals_;
}

void TerrainGenerator::defineTerrain(long x, long y)
{
	Ogre::String filename = mTerrainGroup_->generateFilename(x, y);

	bool exists = Ogre::ResourceGroupManager::getSingleton().resourceExists(
		mTerrainGroup_->getResourceGroup(),
		filename);

	if (exists)
		mTerrainGroup_->defineTerrain(x, y);
	else
	{
		Ogre::Image img;
		getTerrainImage(x % 2 != 0, y % 2 != 0, img);
		mTerrainGroup_->defineTerrain(x, y, &img);

		mTerrainsImported_ = true;
	}
}

void TerrainGenerator::initBlendMaps(Ogre::Terrain * terrain)
{
	Ogre::Real minHeight0 = terrainJson_["maps"][0]["minHeight0"];
	Ogre::Real fadeDist0 = terrainJson_["maps"][0]["fadeDist0"];
	Ogre::Real minHeight1 = terrainJson_["maps"][0]["minHeight1"];
	Ogre::Real fadeDist1 = terrainJson_["maps"][0]["fadeDist1"];

	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();

	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			Ogre::Real val = (height - minHeight0) / fadeDist0;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend0++ = val;

			val = (height - minHeight1) / fadeDist1;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend1++ = val;
		}
	}

	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();
}

void TerrainGenerator::configureTerrainDefaults(Ogre::Light * light)
{
	mTerrainGlobals_->setMaxPixelError(8);
	mTerrainGlobals_->setCompositeMapDistance(3000);

	mTerrainGlobals_->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals_->setCompositeMapAmbient(scnMgn_->getAmbientLight());
	mTerrainGlobals_->setCompositeMapDiffuse(light->getDiffuseColour());

	Ogre::Terrain::ImportData& importData = mTerrainGroup_->getDefaultImportSettings();
	importData.terrainSize = terrainJson_["maps"][0]["terrainSize"];
	importData.worldSize = terrainJson_["maps"][0]["worldSize"];
	importData.inputScale = terrainJson_["maps"][0]["inputScale"];
	importData.minBatchSize = terrainJson_["maps"][0]["minBatchSize"];
	importData.maxBatchSize = terrainJson_["maps"][0]["maxBatchSize"];
	importData.pos = Vector3(0, 0, 0);

	importData.layerList.resize(terrainJson_["maps"][0]["layerListSize"]);

	for (int i = 0; i < importData.layerList.size(); i++)
	{
		importData.layerList[i].worldSize = terrainJson_["maps"][0]["layerWorldSizeList"][i];
		importData.layerList[i].textureNames.push_back(
			terrainJson_["maps"][0]["diffuseSpecularTexturesNames"][i]);
		importData.layerList[i].textureNames.push_back(
			terrainJson_["maps"][0]["normalHeightTexturesNames"][i]);
	}
}

void TerrainGenerator::getTerrainImage(bool flipX, bool flipY, Ogre::Image & img)
{
	img.load(terrainJson_["maps"][0]["terrainImage"], Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();
}