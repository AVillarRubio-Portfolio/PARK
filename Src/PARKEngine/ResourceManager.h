#pragma once
#include "Utils.h"

class ResourceManager 
{
public:
	~ResourceManager();

	static void initInstance();
	static ResourceManager* instance();

	std::vector<std::string> getFontsPaths() { return fontsPaths; };
	std::vector<std::string> getSchemesPaths() { return schemesPaths; };

	json getJsonByKey(const std::string &key);

private:
	ResourceManager();

	static std::unique_ptr<ResourceManager> instance_;

	void loadJsonsFiles(const std::string resourcesPath);
	void loadJson(const std::string &streamFilePath, const std::string &fileName);

	void loadFonts(const std::string resourcesPath);
	void loadSchemes(const std::string resourcesPath);

	const std::string FOLDER_JSON_PATH = "Assets\\jsons\\";
	const std::string FOLDER_FONTS_PATH = "Assets\\HUD\\Fonts";
	const std::string FOLDER_SCHEMES_PATH = "Assets\\HUD\\Schemes";

	//Contenedores de los recursos
	std::map<std::string, json> jsonMap;

	std::vector<std::string> fontsPaths;
	std::vector<std::string> schemesPaths;

};