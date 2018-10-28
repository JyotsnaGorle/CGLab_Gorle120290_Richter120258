#include<model.hpp>
#include "Node.h"
#include "../include/GeometryNode.h"

GeometryNode::GeometryNode(){}
GeometryNode::~GeometryNode() {}


model GeometryNode::getGeometry() {
	return geometry;
}

void GeometryNode::setGeometry(model geometry) {
	this->geometry = geometry;
}
