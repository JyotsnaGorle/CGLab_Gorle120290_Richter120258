#pragma once
#include <Node.h>
#include <Node.h>
class PointLightNode :
	public Node
{
public:
	PointLightNode();
	~PointLightNode();

	glm::vec3 lightColor;
	glm::vec3 lightIntensity;
};

class PointLightNode :
	public Node
{
public:
	PointLightNode();
	~PointLightNode();
};

