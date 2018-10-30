#include "SceneGraph.h"



SceneGraph::SceneGraph()
{ 
	root = new Node;
	root->setLocalTransform(glm::mat4(0,0,0,0,
		0, 0, 0, 0, 
		0, 0, 0, 0, 
		0, 0, 0, 0));
}


SceneGraph::~SceneGraph()
{
	delete(root);
}

string SceneGraph::getName() {
	return name;
}

void SceneGraph::setName(string name) {
	this->name = name;
}

Node SceneGraph::getRoot() {
	return *root;
}

void SceneGraph::setRoot(Node *root) {
	this->root = root;
}