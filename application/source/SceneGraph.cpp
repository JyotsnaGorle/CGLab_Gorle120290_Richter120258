#include "SceneGraph.h"



SceneGraph::SceneGraph()
{ 
	Node *sun = new Node;
	sun->name = "Sun";
	sun->size = 3.0f;
	sun->setDist(glm::vec3{ 0.0f, 0.0f, 0.0f });
	sun->color = { 0.5, 0.5, 0.0 };
	sun->rotationSpeed = 0.1f;
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
	mercury->setDist(glm::vec3{ 0.0f, 0.0f, 4.0f });
	mercury->size = 0.6f;
	mercury->color = glm::vec3{ 0.3, 0.3, 0.0 };
	mercury->rotationSpeed = 0.5f;

	Node *venus = new Node;
	venus->name = "Venus";
	venus->setParent(parent);
	venus->setDist(glm::vec3{ 0.0f, 0.0f, 6.0f });
	venus->size = 0.8f;
	venus->color = glm::vec3{ 0.4, 0.0, 0.0 };
	venus->rotationSpeed = 0.4f;

	Node *earth = new Node;
	earth->name = "earth";
	earth->setParent(parent);
	earth->setDist(glm::vec3{ 0.0f, 0.0f, 9.0f });
	earth->size = 1.0f;
	earth->color = glm::vec3{ 0.0, 0.5, 0.5 };
	earth->rotationSpeed = 0.3f;

	Node *mars = new Node;
	mars->name = "mars";
	mars->setParent(parent);
	mars->setDist(glm::vec3{ 0.0f, 0.0f, 12.0f });
	mars->size = 1.2f;
	mars->color = glm::vec3{ 0.5, 0.0, 0.0 };
	mars->rotationSpeed = 0.2f;

	Node *jupiter = new Node;
	jupiter->name = "earth";
	jupiter->setParent(parent);
	jupiter->setDist(glm::vec3{ 0.0f, 0.0f, 15.0f });
	jupiter->size = 1.5f;
	jupiter->color = glm::vec3{ 0.6, 0.6, 0.0 };
	jupiter->rotationSpeed = 0.1f;

	Node *saturn = new Node;
	saturn->name = "saturn";
	saturn->setParent(parent);
	saturn->setDist(glm::vec3{ 0.0f, 0.0f, 18.0f });
	saturn->size = 2.0f;
	saturn->color = glm::vec3{ 0.0, 0.0, 0.6 };
	saturn->rotationSpeed = 0.1f;

	Node *neptune = new Node;
	neptune->name = "neptune";
	neptune->setParent(parent);
	neptune->setDist(glm::vec3{ 0.0f, 0.0f, 21.0f });
	neptune->size = 2.2f;
	neptune->color = glm::vec3{ 0.0, 0.0, 0.9 };
	neptune->rotationSpeed = 0.1f;

	Node *uranus = new Node;
	uranus->name = "uranus";
	uranus->setParent(parent);
	uranus->setDist(glm::vec3{ 0.0f, 0.0f, 24.0f });
	uranus->size = 2.0f;
	uranus->color = glm::vec3{ 0.0, 0.8, 0.8 };
	uranus->rotationSpeed = 0.1f;

	parent->addChildren(*mercury);
	parent->addChildren(*venus);

	Node *moon = new Node;
	moon->name = "Moon";
	moon->setParent(earth);
	moon->setDist(glm::vec3{ 0.0f, 0.0f, 2.0f });
	moon->size = 0.2f;
	moon->color = glm::vec3{ 0.5, 0.5, 0.5 };
	moon->rotationSpeed = 0.1f;

	earth->addChildren(*moon);

	parent->addChildren(*earth);
	parent->addChildren(*mars);
	parent->addChildren(*jupiter);
	parent->addChildren(*saturn);
	parent->addChildren(*neptune);
	parent->addChildren(*uranus);	
}