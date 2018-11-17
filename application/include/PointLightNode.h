#pragma once
#include <Node.h>
class PointLightNode :
	public Node
{
public:
	PointLightNode();
	~PointLightNode();

	glm::vec3 lightColor;
	float lightIntensity;
};
