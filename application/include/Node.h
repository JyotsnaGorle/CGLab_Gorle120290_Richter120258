#pragma once
#include <glm/glm.hpp>
#include<iostream>
#include<string>
#include<list>
using namespace std;
class Node
{
public:

	std::list<Node> children;
	string name;
	string path;
	int depth;
	glm::mat4 localTransform;
	glm::mat4 worldTransform;
	Node *parent;
	glm::vec3 dist;
	double size;

	Node();
	~Node();

	Node* getParent();

	void setParent(Node *parent);

	Node getChildren(string node_name);


list<Node> getChildrenList();

string getName();
string getPath();

int getDepth();

glm::mat4 getLocalTransform();

void setLocalTransform(glm::mat4 localTransform);
glm::mat4 getWorldTransform();

void setWorldTransform(glm::mat4 worldTransform);

void addChildren(Node childNode);

void removeChildren(string nodeName);

void setDist(glm::vec3 dist);
glm::vec3 getDist();
double getSize();
};

