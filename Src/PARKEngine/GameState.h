#pragma once
#include <vector>
#include <list>
#include <OgreSceneNode.h>

class Component;
class Entity;
class BackgroundImage;
class MeshRenderer;
class SceneManager;

class GameState
{
protected:
	Ogre::SceneNode* stateNode;
	std::list<Component*> components; // Componentes de la escena
	std::list<Entity*> entities; // Entidades de la escena
	std::list<Entity*> removedEntities; //Entidades que se han eliminado en el tick actual
	std::string id; //Nombre de la escena

	Ogre::SceneNode* getStateNode();

	//Elimina la entidad y sus componentes definitivamente
	void removeEntity(Entity* e);
	bool removeAllEntities();

	friend class MeshRenderer;
	friend class BackgroundImage;
	friend class SceneManager;

public:
	GameState(std::string stateID);
	virtual ~GameState();

	virtual void start();

	virtual void update(unsigned int time);
	virtual void render(unsigned int time);
	virtual bool handleInput(unsigned int time);

	// �> A�ade una entidad a la escena
	void addEntity(Entity* e);

	// �> A�ade un grupo de entidades a la escena 
	void addEntities(std::vector<Entity*> ent);

	// �> Elimina una entidad de la escena. Devuelve 'true' si estaba en la escena, 'false' e.o.c
	/*
		El borrado de entidades se retrasa hasta que termine el update actual para así no causar
		problemas al recorrer componentes que ya han sido borrados
	*/
	bool removeEntity(std::string name);

	// 
	void info();

	// �> Devuelve una entidad de la escena
	Entity* getEntity(std::string name);

	//Devuelve una lista de entidades que tengan el componente especificado
	template<typename T>
	std::vector<Entity*> getEntitiesWithComponent()
	{
		std::vector<Entity*> v;
		//Busca el componente
		for (Component* c : components) {
			T* component = dynamic_cast<T*>(c);
			if (component != nullptr)
				v.push_back(component->getEntity());
		}
		return v;
	};

	// �> Devuelve la escena
	std::list<Component*> getComponents();

	//Devuelve el nombre
	std::string getID() { return id; };
};