#include "HeadNode.h"
#include <iostream>

using namespace Rendering;
using namespace GameObjects;
using namespace std;
using namespace glm;
using json = nlohmann::json;

HeadNode::~HeadNode() {
	Destroy();
}

void HeadNode::updateParent(IGameObject* parent) {
	cout << "GameObject " << name << ": The parent of a HeadNode was attempted to be updated. This should not happen." << endl;
}

void HeadNode::addToSceneTree(IGameObject* parent, std::string name, glm::vec3 position, glm::quat orientation, glm::vec3 scale, bool enabled = true) {
	if (parent != nullptr) cout << "GameObject " << name << ": Cannot add HeadNode as a child object. Setting parent to null." << endl;
	this->parent = nullptr;
	this->position = position;
	this->orientation = orientation;
	this->scale = scale;
	this->enabled = enabled;
	this->matricesValid = false;
}

void HeadNode::updateLocalMatrices() {
	toParentMatrix = mat4();
	toWorldMatrix = mat4();
	matricesValid = true;
}

json HeadNode::GetJSON() {
	json j = IGameObject::GetJSON();
	j["Type"] = "HeadNode";
	return j;
}