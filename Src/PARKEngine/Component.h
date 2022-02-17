#pragma once
#include "Utils.h"
#include "Messages.h"
#include "ComponentCreator.h"

using namespace Architecture;

//enum  Name{ TransformComp, MeshRendererComp, RigidbodyComp, CameraComp, TerrainComp  }; 
//Cada vez que se cree un Componente , se tiene que añadir a la lista de nombres.

/*
Clase Componente:
-> Define un comportamiento específico.
-> Contiene métodos para pintarse, gestionar eventos y actualizar lógica.
-> Tiene un flag de actividad y un puntero a la entidad a la que está asociado.
-> Hereda de Listener y Emitter para que así los componentes hijos puedan redefinirlos
* Esta clase padre es abstracta, ya que no define ningún comportamiento.
*/

class Entity;


class Component :public Listener, public Emitter
{
protected:
	//Flag de actividad
	bool active_;
	//Puntero a la entidad
	Entity* entity_;


	template<typename T>
	bool addParameter(T& callbackParam, json value)
	{
		if (!value.empty())
		{
			callbackParam = value;
			return true;
		}
		return false;
	}

public:
	//Constructora por defecto
	Component();

	virtual void load(json file) {};

	//Método para que los componentes e inicialicen con información de sus componentes hermanos
	virtual void start() {};

	//Devuelve información sobre el componente. Cada uno puede redefinirlo a su gusto
	virtual std::string getInfo() { return "This component hasn't got an implementation for 'getInfo()' \n"; };

	//Métodos principales (renderizado, lógica y eventos)
	virtual void render(unsigned int time) {};
	virtual void update(unsigned int time) {};
	virtual bool handleEvent(unsigned int time) { return false; };

	//Guardar Estado
	virtual void save(json& file) {};

	//Returns activity flag
	bool isActive() { return active_; };
	//Sets activity flag
	void setActive(bool b) { active_ = b; };

	//Returns pointer to the entity
	void setEntity(Entity* ent) { entity_ = ent; };
	//Sets pointer to the entity
	Entity* getEntity() { return entity_; }
	//Releases the entity
	void releaseEntity();

	//Gets a specific component within the same entity
	template<typename T>
	T* getBrotherComponent()
	{
		return getEntity()->getComponent<T>();
	}

	//Destructora
	virtual ~Component();
};
