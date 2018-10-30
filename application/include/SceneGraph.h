#pragma once
#include<iostream>
#include<string>
#include "../include/Node.h"

using namespace std;
class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();

	string name;
	Node *root;

	string getName();
	Node getRoot();
	void setName(string name);
	void setRoot(Node *root);
};

