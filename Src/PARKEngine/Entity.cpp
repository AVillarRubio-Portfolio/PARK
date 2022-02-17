#include "Entity.h"
#include "Component.h"

Entity::Entity():name_("undefined"), active_(true)
{
	
}

Entity::Entity(std::vector<Component*> comps, std::string name):name_(name), active_(true)
{
	//Creamos la entidad con los componentes dados, llamando a addComponent
	for (Component* c : comps)
		addComponent(c);
}

void Entity::start() 
{
	for (Component* c : components_)
		if(c->isActive())
			c->start();
}


void Entity::addComponent(Component* comp)
{
	//Metemos el componente en el vector y avisamos al componente desde nuestra entidad
	components_.push_back(comp);
	comp->setEntity(this);
}

bool Entity::delComponent(Component* comp)
{
	//Buscamos el componente en cuestión
	std::vector<Component*>::iterator it;
	it = std::find(components_.begin(), components_.end(), comp);
	//Si lo encuentra, lo elimina de su lista y lo notifica
	if (it != components_.end())
	{
		components_.erase(it);
		return true;
	}
	return false;
}

void Entity::setActive(bool b)
{
	active_ = b;
	for (Component* c : components_)
		c->setActive(b);
}

Entity::~Entity()
{
	std::cout << "Destructora de Entity: " << name_ << std::endl;
}
