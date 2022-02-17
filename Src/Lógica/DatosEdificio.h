#pragma once
#include <PARKEngine/Component.h>

struct Datos
{
	//Esta variable es importante ya que es la clave por 
	//la que se busca la atraccion
	std::string bName; //Nombre de la atracción

	int price_;
	int LockedPrice_;
	bool Locked_;

	int entryCost_;
	//Estos son los valores que un edificio modifica de un NPC
	//si estos son negativos disminuiran la necesidad del NPC
	//en caso de ser nulos no afectan y positivo la aumenta
	int PeePeeRestore_;
	int HungryRestore_;
	int funRestore_;

	//Tamaños para saber si se puede construir
	Ogre::Vector2 tam;
	int height_;
	Ogre::Vector2 entry;
	Ogre::Vector2 exit;

	int maxCola_; //Tamaño máximo de la cola, si esta está llena no se podrá acceder a ella
	int duration_; //En milisegundos, La de Ornament debe ser 0
	int actDuration_; //Duración que lleva por el momento la atracción.
	int capacity_; //La de ornament debe ser 0
};

class DatosEdificio : public Component
{
public:
	
	DatosEdificio();
	~DatosEdificio();

private:
	

	virtual void load(json file);
	virtual void update(unsigned int time) {};
	virtual bool handleEvent(unsigned int time) { return true; };
	virtual void receive(Message* msg) {};

	std::map<std::string, Datos*> datos_;


public:
	Datos* findData(std::string s);

	int getPrice(std::string s) { 
		Datos* d = findData(s);
		return (d->Locked_) ? d->LockedPrice_ : d->price_;
	};

	bool getLocked(std::string s) { return findData(s)->Locked_; };
	void setLocked(std::string s, bool b);

	int getPeePeeRestore(std::string s) { return  findData(s)->PeePeeRestore_; };
	int getHungryRestore(std::string s) { return  findData(s)->HungryRestore_; };
	int getFunRestore(std::string s) { return  findData(s)->funRestore_; };
	Ogre::Vector2 getTam(std::string s) { return  findData(s)->tam; };
	int getHeight(std::string s) { return  findData(s)->height_; };
	Ogre::Vector2 getEntry(std::string s) { return findData(s)->entry; };
	Ogre::Vector2 getExit(std::string s) { return findData(s)->exit; };
	int getMaxCola(std::string s) { return  findData(s)->maxCola_; };
	int getDuration(std::string s) { return  findData(s)->duration_; };
	int getActDuration(std::string s) { return  findData(s)->actDuration_; };
	int getCapacity(std::string s) { return  findData(s)->capacity_; };
	virtual std::string getInfo() { return "DatosEdificio"; };

};
REGISTER_TYPE(DatosEdificio)

