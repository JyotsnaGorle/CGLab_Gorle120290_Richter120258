#include<model.hpp>
#include "Node.h"
class GeometryNode: Node {
	model geometry;
	model getGeometry() {
		return geometry;
	}
	void setGeometry(model geometry) {
		this->geometry = geometry;
	}
};