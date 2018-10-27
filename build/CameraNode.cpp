#include "CameraNode.h"

bool CameraNode::getPerspective() {
	return isPerspective;
}
bool CameraNode::getEnabled() {
	return isEnabled;
}
void CameraNode::setEnabled(bool value) {
	this->isEnabled = value;
}
glm::mat4 CameraNode::getProjectionMatrix() {
	return projectionMatrix;
}
void CameraNode::setProjectionMatrix(glm::mat4 matrix) {
	this->projectionMatrix = projectionMatrix;
}