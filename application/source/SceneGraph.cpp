#include "SceneGraph.h"



SceneGraph::SceneGraph()
{ 
	Node *sun = new Node;
	sun->name = "Sun";
	sun->size = 3.0f;
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
	mercury->size = 0.5f;

	Node *venus = new Node;
	venus->name = "Venus";
	venus->setParent(parent);
	venus->setDist(glm::vec3{ 0.0f, 0.0f, 10.0f });
	venus->size = 1.2f;

	Node *earth = new Node;
	earth->name = "earth";
	earth->setParent(parent);
	earth->setDist(glm::vec3{ 0.0f, 0.0f, 15.0f });
	earth->size = 1.8f;

	Node *mars = new Node;
	mars->name = "mars";
	mars->setParent(parent);
	mars->setDist(glm::vec3{ 0.0f, 0.0f, 20.0f });
	mars->size = 2.2f;

	Node *jupiter = new Node;
	jupiter->name = "earth";
	jupiter->setParent(parent);
	jupiter->setDist(glm::vec3{ 0.0f, 0.0f, 25.0f });
	jupiter->size = 2.5f;

	Node *saturn = new Node;
	saturn->name = "saturn";
	saturn->setParent(parent);
	saturn->setDist(glm::vec3{ 0.0f, 0.0f, 30.0f });
	saturn->size = 3.0f;

	Node *neptune = new Node;
	neptune->name = "neptune";
	neptune->setParent(parent);
	neptune->setDist(glm::vec3{ 0.0f, 0.0f, 35.0f });
	neptune->size = 3.2f;

	Node *uranus = new Node;
	uranus->name = "uranus";
	uranus->setParent(parent);
	uranus->setDist(glm::vec3{ 0.0f, 0.0f, 40.0f });
	uranus->size = 3.0f;

	parent->addChildren(*mercury);
	parent->addChildren(*venus);

	Node *moon = new Node;
	moon->name = "Moon";
	moon->setParent(earth);
	moon->setDist(glm::vec3{ 0.0f, 0.0f, 3.5f });
	moon->size = 0.4f;

	earth->addChildren(*moon);

	parent->addChildren(*earth);
	parent->addChildren(*mars);
	parent->addChildren(*jupiter);
	parent->addChildren(*saturn);
	parent->addChildren(*neptune);
	parent->addChildren(*uranus);

	
}