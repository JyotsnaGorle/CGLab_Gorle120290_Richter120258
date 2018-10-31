#include "Node.h"

	Node::Node(){}
	Node::~Node() {
		/*delete(parent);*/
	}

	Node* Node::getParent() {
		return parent;
	}

	void Node::setParent(Node *parent) {
		this->parent = parent;
	}

	Node Node::getChildren(string node_name) {
		for (auto each : children) {
			if (each.name == node_name)
				return each;
		}
	}

	list<Node> Node::getChildrenList() {
		return children;
	}

	string Node::getName() {
		return name;
	}

	string Node::getPath() {
		return path;
	}

	int Node::getDepth() {
		return depth;
	}

	glm::mat4 Node::getLocalTransform() {
		return this->localTransform;
	}

	void Node::setLocalTransform(glm::mat4 localTransform) {
		this->localTransform = localTransform;
	}

	glm::mat4 Node::getWorldTransform() {
		return worldTransform;
	}

	void Node::setWorldTransform(glm::mat4 worldTransform) {
		this->worldTransform = worldTransform;
	}

	void Node::addChildren(Node childNode) {
		this->children.insert(children.end(), childNode);
	}

	void Node::removeChildren(string nodeName) {
		//this->children.remove_if([&](Node each) { return each.name.compare(nodeName) == 0; });
	}

	void Node::setDist(glm::vec3 dist) {
		this->dist = dist;
	};
	glm::vec3 Node::getDist() {
		return this->dist;
	};
	double Node::getSize() {
		return this->size;
	}
