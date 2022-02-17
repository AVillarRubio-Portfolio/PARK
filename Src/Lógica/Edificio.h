#pragma once
#include <PARKEngine/Component.h>
#include "DatosEdificio.h"

class Node;

class Edificio : public Component
{
public:
	bool Edificio::operator ==(const Edificio& a) {
		return datos->bName == datos->bName;
	}
	//El tipo ornament no tiene ni entrada ni salida, su cola es de 0 y no cambia valores de necesidades
	enum BuildingType { Ornament, Amusement, Restaurant, Toilet, other };
private:
	Node* EntryNode;
	Entity* EntryEntity;
	Node* ExitNode;
	Entity* ExitEntity;

	BuildingType type_; //El tipo d ela atracción

	std::queue<Entity*> cola;
	std::list<Entity*> rideing;

	std::list<Entity*> nodes_;

	Datos* datos;

public:
	Edificio();
	~Edificio();

	//Actualización del componente
	virtual void start();
	virtual void update(unsigned int time);

	bool encolar(Entity* e);
	void montar();
	void sacar();

	void load(json file);

	//GETTERS
	std::list<Entity*> getNodes() { return nodes_; };
	virtual std::string getInfo() { return "Edificio"; };

	int getPrice() { return datos->price_; };
	int getEntryCost() { return datos->entryCost_; };
	//Dice si la cola del edificio está llena o no
	bool isFull() { return (cola.size() > datos->maxCola_); };

	BuildingType getType() { return type_; };

	int getPeePeeValue() { return datos->PeePeeRestore_; };
	int getHungryValue() { return datos->HungryRestore_; };
	int getFunValue() { return datos->funRestore_; };
	//Para obtener las coordenadas de forma individual
	int getTamX() { return datos->tam.x; };
	int getTamY() { return datos->tam.y; };

	int getEntryX() { return datos->entry.x; };
	int getEntryY() { return datos->entry.y; };

	int getExitX() { return datos->exit.x; };
	int getExitY() { return datos->exit.y; };
	//
	Ogre::Vector2 getTam() { return datos->tam; };
	Ogre::Vector2 getEntry() { return datos->entry; };
	Ogre::Vector2 getExit() { return datos->exit; };
	Node* getEntryNode() { return EntryNode; };
	Entity* getEntryEntity() { return EntryEntity; };
	Node* getExitNode() { return ExitNode; };
	Entity* getExitEntity() { return ExitEntity; };

	int getHeight() { return datos->height_; };

	//Devuelve la duración de la atracción en milisegundos
	int getDuration() { return datos->duration_; };

	std::string getBuildingName() { return datos->bName; };

	//SETTERS
	void getPeePeeValue(int P) { datos->PeePeeRestore_ = P; };
	void setHungryValue(int H) { datos->HungryRestore_ = H; };
	void setFunValue(int F) { datos->funRestore_ = F; };
	//El parametro d es en segundos y automaticamente lo pasa a milisegundos que es como trabaja internamente
	void setDuration(int D) { datos->duration_ = D * 1000; };

	void setNodes(std::list<Entity*> n) { nodes_ = n; };
	void setEntryNode(Node* e) { EntryNode = e; };
	void setEntryEntity(Entity* e) { EntryEntity = e; }
	void setExitNode(Node* e) { ExitNode = e; };
	void setExitEntity(Entity* e) { ExitEntity = e; }
private:
	//Es privado porque solo se debe establecer al Establecer de json la atracción y no en ejecución
	void setType(BuildingType t) { type_ = t; };
	//Es mejor que en el Json al ser solo 4 tipos de valores lea un int para no tener que ir interpretando el string constantemente
	void setTypeByInt(int t) { if (t < sizeof(BuildingType)) type_ = BuildingType(t); };
	//establece el nombre del edificio
	void setName(std::string s) { datos->bName = s; };
};

REGISTER_TYPE(Edificio);