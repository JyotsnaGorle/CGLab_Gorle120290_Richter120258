#include "SceneGraph.h"



SceneGraph::SceneGraph()
{ 
}


SceneGraph::~SceneGraph()
{
	delete(root);
	delete(sceneGraphInstance);
}

SceneGraph *SceneGraph::getClassInstance()
{
	return *sceneGraphInstance;
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