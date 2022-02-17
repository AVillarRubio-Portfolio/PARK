#pragma once
#include <vector>
#include <string>
/*
Clase Entidad:
-> Es un simple contenedor de omponentes, los cuales definen su comportamiento.
-> Hereda de Listener (para recibir mensajes) y de Emitter (para enviarlos).
-> Tiene una referencia a su RigidBody (de physx) para el motor físico
*/
class Component;
class Entity 
{
public:
	//Constructora por defecto
	Entity();
	//Constructora con los componentes ya dados en un vector
	Entity(std::vector<Component*> comps, std::string name = "undefined"); 

	//Llama al start de todos sus componentes
	void start();
	 //Añade un componente a la entidad
	void addComponent(Component* comp);
	//Elimina un componente de la entidad. Devuelve "true" si la entidad tenía ese componente, "false" e.o.c
	bool delComponent(Component* comp);

	//Devuelve el componente del tipo especificado
	template<typename T>
	T* getComponent()
	{
		//Busca el componente
		for (Component* c : components_) {
			T* component = dynamic_cast<T*>(c);
			if (component != nullptr)
				return component;
		}
		//Si no lo encuentra, devuelve nullptr
		return nullptr;
	};
	
	//Devueve la lista de componentes de la Entidad
	std::vector<Component*> getComponents() { return components_; }

	//Devuelve el nombre de la propia entidad
	std::string getName() { return name_; };

	//Otorga un nombre a la entidad
	void setName(std::string name) { name_ = name; };

	//Pone activa la entidad
	void setActive(bool b);

	//Indica si la entidad está activa
	bool isActive() { return active_; };
	
	//Destructora
	virtual ~Entity();
private:
	//Vector de componentes genérico (no se diferencian por el tipo de componente)
	std::vector<Component*> components_;
	std::string name_;
	bool active_;
};
