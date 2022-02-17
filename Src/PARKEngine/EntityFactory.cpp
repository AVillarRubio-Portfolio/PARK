#include "EntityFactory.h"
#include "Entity.h"
#include "Component.h"
#include "ResourceManager.h"
#include "GameState.h"
#include "SceneManager.h"

std::unique_ptr<EntityFactory> EntityFactory::instance_;

EntityFactory::EntityFactory()
{
}

EntityFactory::~EntityFactory()
{
	std::cout << "Destructora de EntityFactory" << std::endl;
}



EntityFactory* EntityFactory::Instance()
{
	if (instance_.get() == nullptr)
		instance_.reset(new EntityFactory());
	return instance_.get();
}


void EntityFactory::registerType(std::string creatorName, BaseCreator* pCreator)
{
	creators()[creatorName] = pCreator;
}


std::vector<Entity*> EntityFactory::createEntities(std::string ID)
{
	//Carga el archivo con los datos del tipo de estado solicitado
	json file = ResourceManager::instance()->getJsonByKey(ID + ".json");
	assert(file != nullptr);

	//Almacena el nombre del GameState que estamos construyendo.
	//currentlyCreatingState = currState;

	std::vector<Entity*> entities;
	std::map<std::string, Component*> dic;

	
	for (json ent : file["Entities"])
	{
		Entity* entity = new Entity();
		entity->setName(ent["name"]);

		for (json comp : ent["Components"])
		{
			Component* c = createComponent(comp["name"]);
			entity->addComponent(c);
			c->load(comp);
			dic[comp["name"]] = c;
			for (json j : comp["Listeners"])
			{
				c->registerListener(dic[j["name"]]);
			}
		}

		entities.push_back(entity);

		//Registramos a todos los componentes como listeners
		for (Component* c : entity->getComponents())
			for (Component* c2 : entity->getComponents())
				if (c != c2)
					c->registerListener(c2);		
	}

	return entities;
}


Entity* EntityFactory::createEntityFromBlueprint(std::string name)
{
	Entity* entity = new Entity();
	entity->setName(name);

	std::cout << "Creando Entidad: " << entity->getName() << std::endl;

	json blueprints = ResourceManager::instance()->getJsonByKey("Entities.json");
	assert(blueprints != nullptr);

	for (json comp : blueprints[name])
	{
		//create component nos devolver el componente si existe o nullptr en caso contrario
		Component* c = createComponent(comp["name"]);

		//Si existe añadimos el componente a la entidad, en caso contrario no hace nada.
		if (c != nullptr) {
			entity->addComponent(c);

			if (comp.find("parameters") != comp.end())
				c->load(comp["parameters"]);
		}
	}

	//Registramos a todos los componentes como listeners
	for (Component* c : entity->getComponents())
		for (Component* c2 : entity->getComponents())
			if (c != c2)
				c->registerListener(c2);
	
	return entity;
}

/*
GameState * EntityFactory::get_currentState()
{
	return currentlyCreatingState;
}
*/


Component* EntityFactory::createComponent(std::string name)
{
	auto it = creators().find(name);

	//RECU
	//Si no encuentra el componente
	if (it == creators().end()) return nullptr;
	else {
		BaseCreator* compCreator = it->second;
		return compCreator->createComponent();
	}
}

std::map<std::string, BaseCreator*>& EntityFactory::creators()
{
	static std::map<std::string, BaseCreator*>* map = new std::map<std::string, BaseCreator*>();
		   
	return *map;
}