#include "HeadNode.h"
#include <iostream>

using namespace Rendering;
using namespace GameObjects;
using namespace std;
using namespace glm;
using json = nlohmann::json;

HeadNode::HeadNode(string name) : IGameObject(name) {

}

HeadNode::~HeadNode() {
	Destroy();
}

void HeadNode::UpdateParent(IGameObject* parent) {
	cout << "GameObject " << name << ": The parent of a HeadNode was attempted to be updated. This should not happen." << endl;
}

void HeadNode::AddToSceneTree(IGameObject* parent, vec3 position, quat orientation, vec3 scale, bool enabled = true) {
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