#include "SceneGraph.h"



SceneGraph::SceneGraph()
{ 
	Node *sun = new Node;
	sun->name = "Sun";
	sun->setDist(glm::vec3{ 0.0f, 0.0f, 0.0f });
	this->root = sun;
	
	addPLanets(sun);
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

void SceneGraph::addPLanets(Node *parent) {
	Node *mercury = new Node;
	mercury->name = "Mercury";
	mercury->setParent(parent);
	mercury->setDist(glm::vec3{ 0.0f, 0.0f, 5.0f });

	Node *venus = new Node;
	venus->name = "Venus";
	venus->setParent(parent);
	venus->setDist(glm::vec3{ 0.0f, 0.0f, 10.0f });

	Node *earth = new Node;
	earth->name = "earth";
	earth->setParent(parent);
	earth->setDist(glm::vec3{ 0.0f, 0.0f, 15.0f });

	Node *mars = new Node;
	mars->name = "mars";
	mars->setParent(parent);
	mars->setDist(glm::vec3{ 0.0f, 0.0f, 20.0f });

	Node *jupiter = new Node;
	jupiter->name = "earth";
	jupiter->setParent(parent);
	jupiter->setDist(glm::vec3{ 0.0f, 0.0f, 25.0f });

	Node *saturn = new Node;
	saturn->name = "saturn";
	saturn->setParent(parent);
	saturn->setDist(glm::vec3{ 0.0f, 0.0f, 30.0f });

	Node *neptune = new Node;
	neptune->name = "neptune";
	neptune->setParent(parent);
	neptune->setDist(glm::vec3{ 0.0f, 0.0f, 35.0f });

	Node *uranus = new Node;
	uranus->name = "uranus";
	uranus->setParent(parent);
	uranus->setDist(glm::vec3{ 0.0f, 0.0f, 40.0f });

	parent->addChildren(*mercury);
	parent->addChildren(*venus);
	parent->addChildren(*earth);
	parent->addChildren(*mars);
	parent->addChildren(*jupiter);
	parent->addChildren(*saturn);
	parent->addChildren(*neptune);
	parent->addChildren(*uranus);
}