#pragma once
#include "Node.h"
#include <glm/glm.hpp>
#include<iostream>
#include<string>
#include<list>

class CameraNode : Node
{
	bool isPerspective;
	bool isEnabled;
	glm::mat4 projectionMatrix;

	bool getPerspective();
	bool getEnabled();
	void setEnabled(bool value);
	glm::mat4 getProjectionMatrix();
	void setProjectionMatrix(glm::mat4 matrix);
};

