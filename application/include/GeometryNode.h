#pragma 
#include "Node.h"
class GeometryNode : Node{
	
public:
	GeometryNode();
	~GeometryNode();
	model geometry;
	model getGeometry();
	void setGeometry(model geometry);
};