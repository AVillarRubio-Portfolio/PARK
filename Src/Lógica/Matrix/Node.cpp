#include "Node.h"

Node::Node() : posMatrix_({ 0, 0 }), nodeType_(NodeType::Empty)
{
}

Node::~Node()
{
	std::cout << "Destructora de Node" << std::endl;
}

void Node::load(json file)
{
	nodeType_ = NodeType::Empty;
}

void Node::setMatrixPos(int i, int j)
{
	posMatrix_ = Ogre::Vector2(i, j);
}

Vector2 Node::getMatrixPos()
{
	return posMatrix_;
}

Node::NodeType Node::getType()
{
	return nodeType_;
}

void Node::setType(NodeType t)
{
	nodeType_ = t;
}
