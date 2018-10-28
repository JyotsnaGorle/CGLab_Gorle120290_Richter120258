#pragma once
#include<iostream>
#include<string>
#include "Node.h"

using namespace std;
class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();

	string name;
	Node *root;

	string getName();
	void setName(string name);
	Node getRoot();
	void setRoot(Node *root);
};

