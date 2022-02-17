#pragma once
#include "PARKEngine/Component.h"

using namespace std;

class Node : public Component
{
public:
	enum NodeType { Empty, Road, EntryRoad, ExitRoad, Building };
	Node();
	~Node();

	virtual void load(json file);

	virtual void render(unsigned int time) {};
	virtual void update(unsigned int time) {};
	virtual bool handleEvent(unsigned int time) { return false; };
	virtual void receive(Message* msg) {};

	void setMatrixPos(int i, int j);
	//Devuelve la posición en la matriz (fila, columna)
	Vector2 getMatrixPos();

	NodeType getType();
	void setType(NodeType t);

	virtual std::string getInfo() { return "Node"; };

private:
	Ogre::Vector2 posMatrix_;
	NodeType nodeType_;

	//Componente asociado al edificio

};

REGISTER_TYPE(Node)