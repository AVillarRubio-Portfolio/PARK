#pragma once
#include <string>
#include <memory>
#include <dirent.h>
#include <OgreRay.h>

//Espacio de nombres de Ogre (predeclaración de clases)
namespace Ogre
{
	class Light;
	class Plane;
	class Camera;
	class Entity;
	class RenderWindow;
	class SceneManager;
	class Root;
	class SceneNode;
	class MeshManager;
	class ResourceGroupManager;
	class TextureManager;
	class FileSystemLayer;
	class AnimationState;
};

class TerrainGenerator;
class Entity;

class OgreManager
{
public:
	//"Constructora" de la instancia
	static void initInstance(std::string initFileJson);

	//Devuelve la instancia
	static OgreManager* instance();

	//Destructora
	~OgreManager();

	//Renderiza la escena (deltaTime debe estar en segundos)
	void render(unsigned int deltaTime);
	//Lanza un rayo desde la cámara hacia la posición del ratón y devuelve la entidad con la que colisiona y el punto exacto
	std::pair<Entity*, Ogre::Vector3> raycastToMouse(std::string ignoreEntityName = "");
	//Lanza un rayo desde la cámara hacia la posición de la pantalla indicada y devuelve la entidad con la que colisiona y el punto exacto
	std::pair<Entity*, Ogre::Vector3> raycast(float screenX = 0.5, float screenY = 0.5, std::string ignoreEntityName = "");

	//Devuelve las anchura del viewport
	float getWindowSizeX();
	//Devuelve las altura del viewport
	float getWindowSizeY();

private:
	//Clases amigas: necesitan acceder a los managers de Ogre
	friend class ResourceManager;
	friend class InputManager;
	friend class HUDManager;
	friend class MeshRenderer;
	friend class Camera;
	friend class Terrain;
	friend class GameState;
	friend class BackgroundImage;

	//Constructora privada
	OgreManager(std::string initFileJson);
	//Instancia
	static std::unique_ptr<OgreManager> instance_;
	

	//ATRIBUTOS
	//Variable que toma la ventana de Ogre para aplicarla a la de SDL
	unsigned long hWnd;
	
	//Raíz del árbol, gestor y ventana
	Ogre::Root *root_;
	Ogre::SceneManager *sceneMgr_;
	Ogre::RenderWindow *window_;
	
	//Luces, cámara y acc... plano
	Ogre::Light* light_;
	Ogre::Camera* camera_;
	Ogre::Viewport* viewport_;

	Ogre::Plane* plane_;

	//GETTERS DE LOS MANAGERS (solo los pueden coger los amigos)
	//Devuelve el gestor de escenas de Ogre
	Ogre::SceneManager * getSceneManager();
	//Devuelve el gestor de mallas de Ogre
	Ogre::MeshManager * getMeshManager();
	//Devuelve el gestor de recursos de Ogre
	Ogre::ResourceGroupManager* getResourceGroupManager();
	//Devuelve el gestor de texturas de Ogre
	Ogre::TextureManager* getTextureManager();

	//OTROS GETTERS
	//Devuelve la ventana de Ogre
	Ogre::RenderWindow* getWindow() { return window_; };
	//Devuelve la luz
	Ogre::Light* getLight() { return light_; };
	//Devuelve la ventana
	unsigned long gethWnd() { return hWnd; }


	//Recu
	void messagePump();

	//RECURSOS
	//Initialises window
	void initWindow(std::string initFileJson);
	//Elimina el sistema de archivos
	void deleteFileSystemLayer(Ogre::FileSystemLayer* fsLayer);

	Ogre::Camera* getCamera() { return camera_; }

	//Crea una cámara
	Ogre::Camera* createCamera(std::string name, float NearClipDist, float FarClipDist,
		bool autoAspectRatio, float AspectRatio = 1.3);
	//Crea un plano
	Ogre::Entity* createPlane(std::string name, std::string MaterialName, float width, float height, int Xsegments, int Ysegments,
		Ogre::SceneNode* FatherNode);
	//Crea un terreno
	TerrainGenerator* createTerrain(std::string terrainFile);


	//MÉTODOS AUXILIARES
	//Gets a certain entity from an OgreSceneNode
	Entity * getEntityFromNode(Ogre::SceneNode * n);
};
