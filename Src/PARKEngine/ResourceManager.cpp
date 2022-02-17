#include "ResourceManager.h"

//CEGUI
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

//OGRE
#include "OgreIncludes.h"

//NUESTRO
#include "OgreManager.h"



std::unique_ptr<ResourceManager> ResourceManager::instance_;

ResourceManager::ResourceManager()
{
	//INITIALISES ALL RESOURCES
	Ogre::ConfigFile cf;

	Ogre::FileSystemLayer* mFSLayer = OGRE_NEW Ogre::FileSystemLayer("cfLayerSystem");

	//Este metodo ya agrega '_d' si se compila en debug
	Ogre::String resourcesPath = mFSLayer->getConfigFilePath("resources.cfg");

	std::string jsonPath;

	if (Ogre::FileSystemLayer::fileExists(resourcesPath))
		cf.load(resourcesPath);

	else
		throw::std::invalid_argument("Archivo resources.cfg no encontrado");

	Ogre::String sec, type, arch;

	// Recorre todos los grupos de recursos definidos en el archivo
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;

	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci)
	{
		sec = seci->first;

		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		// Recorre todos los path
		for (i = settings.begin(); i != settings.end(); i++)
		{
			type = i->first;
			std::string auxPath = resourcesPath;
			auxPath.erase(auxPath.find_last_of("\\") + 1, auxPath.size() - 1);
			jsonPath = auxPath;
			arch = auxPath + Ogre::FileSystemLayer::resolveBundlePath(i->second);

			//Va agregando las ubicaciones definidas en el cfg
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}
	OGRE_DELETE mFSLayer;


	loadFonts(jsonPath);
	loadSchemes(jsonPath);
	loadJsonsFiles(jsonPath);
}

ResourceManager::~ResourceManager()
{
	std::cout << "Destructora de ResourceManager" << std::endl;
	instance_.release();
}

void ResourceManager::initInstance()
{
	//Creamos la instancia
	if (instance_.get() == nullptr)
		instance_.reset(new ResourceManager());
}

ResourceManager * ResourceManager::instance()
{
	assert(instance_.get() != nullptr);
	return instance_.get();
}


json ResourceManager::getJsonByKey(const std::string & key)
{
	std::map<std::string, json>::iterator it = jsonMap.find(key);

	//Si el puntero devuelve end() no ha encontrado ese archivo
	if (it != jsonMap.end()) return (*it).second;
	else return nullptr;
}


void ResourceManager::loadJsonsFiles(const std::string resourcesPath)
{
	//Se le añade al path exes la carpeta Assets\\jsons
	std::string filePath = resourcesPath + FOLDER_JSON_PATH;
	const char* filePathC = filePath.c_str();

	DIR *dir;
	struct dirent *ent;

	/*Se busca todos los archivos que existen el el directorio filePathPc*/
	if ((dir = opendir(filePathC)) != NULL) {

		while ((ent = readdir(dir)) != NULL) {
			//No se procesa el dir actual ni el padre
			if ((strcmp(ent->d_name, ".") == 1) && (strcmp(ent->d_name, "..") == 1)) {

				//Para los demas se abren, se leen y se pasan al tipo json
				std::fstream fStream;
				fStream.open(filePath + ent->d_name);
				loadJson(filePath + ent->d_name, ent->d_name);
				fStream.close();
			}
		}
		closedir(dir);
	}
}

void ResourceManager::loadJson(const std::string & streamFilePath, const std::string & fileName)
{
	std::fstream stream_(streamFilePath);
	json j;
	stream_ >> j;
	//Se inserta los json que se van encontrando en el dir al diccionario
	jsonMap.insert(std::pair<std::string, json>(fileName, j));
}

void ResourceManager::loadFonts(const std::string resourcesPath)
{
	//Se le añade al path exes la carpeta Assets\\HUD\\Fonts
	std::string filePath = resourcesPath + FOLDER_FONTS_PATH;
	const char* filePathC = filePath.c_str();

	DIR *dir;
	struct dirent *ent;
	std::string fontExt = ".font";

	//Se busca todos los archivos que existen el el directorio filePathPc
	if ((dir = opendir(filePathC)) != NULL) {

		while ((ent = readdir(dir)) != NULL) {
			//No se procesa el dir actual ni el padre
			if ((strcmp(ent->d_name, ".") == 1) && (strcmp(ent->d_name, "..") == 1)) {
				std::string fName = ent->d_name;
				if (fName.find(fontExt, (fName.size() - fontExt.size())) != std::string::npos)
					fontsPaths.push_back(fName);
			}
		}
		closedir(dir);
	}
}

void ResourceManager::loadSchemes(const std::string resourcesPath)
{
	//Se le añade al path exes la carpeta Assets\\HUD\\Fonts
	std::string filePath = resourcesPath + FOLDER_SCHEMES_PATH;
	const char* filePathC = filePath.c_str();

	DIR *dir;
	struct dirent *ent;
	std::string schemeExt = ".scheme";

	//Se busca todos los archivos que existen el el directorio filePathPc
	if ((dir = opendir(filePathC)) != NULL) {

		while ((ent = readdir(dir)) != NULL) {
			//No se procesa el dir actual ni el padre
			if ((strcmp(ent->d_name, ".") == 1) && (strcmp(ent->d_name, "..") == 1)) {
				std::string fName = ent->d_name;
				if (fName.find(schemeExt, (fName.size() - schemeExt.size())) != std::string::npos)
					schemesPaths.push_back(fName);
			}
		}
		closedir(dir);
	}
}


