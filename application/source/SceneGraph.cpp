#include "SceneGraph.h"



SceneGraph::SceneGraph()
{ 
	Node *sun = new Node;
	sun->name = "Sun";
	sun->setLocalTransform(glm::mat4(0,0,0,0,
		0, 0, 0, 0, 
		0, 0, 0, 0, 
		0, 0, 0, 0));
	setRoot(sun);
	Node *mercury = new Node;
	mercury->name = "Mercury";
	mercury->setParent(sun);
	mercury->setLocalTransform(glm::mat4(0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 5.0f,
		0, 0, 0, 0));
	sun->addChildren(*mercury);
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