#pragma once
#include "PARKEngine/Component.h"

class Entity;
class Node;

using namespace std;
class Matrix : public Component
{
public:
	Matrix();
	~Matrix();

	virtual void start();

	virtual void load(json file);

	//Returns a string with matrix info
	virtual std::string getDetailedInfo();
	virtual std::string getInfo() { return "Matrix -> More on getDetailedInfo()"; };

	//Devuelve la matriz de nodos
	vector<vector<Entity*>> getMatrix();
	//Devuelve la entrada
	Node* getEntrance() { return entrance_; };
	//Devuelve la entidad nodo en una posición de la matriz
	Entity* getEntityNode(int i, int j);
	//Devuelve los adyacentes de una entidad con un tamaño x-y
	list<Entity*> getAdj(Entity* e, int x, int y);

	//Devuelve adyacentes especificando indice
	Entity* getEntityNode(int index);
	//Pa pillar el tamaño
	int getSize(int i) { if (i == 0) return matrix_.size(); else return matrix_.at(0).size(); };

	Ogre::Vector3 getNodeSize();
	bool limits(int i, int j);

private:
	Ogre::Vector2 mSize_;
	Ogre::Vector3 nSize_;
	vector<vector<Entity*>> matrix_;
	//Entrance to the PARK
	Node* entrance_;
	int entX, entY;
	std::list<Component*> comps;
	
	//Crea la matriz de nodos
	void createMatrix();
	Vector3 getPosIni();

	json matrixFile;
};

REGISTER_TYPE(Matrix)