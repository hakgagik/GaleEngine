#include "IGameObject.h"
#include <iostream>

using namespace Rendering;
using namespace GameObjects;
using namespace std;
using namespace glm;
using json = nlohmann::json;

IGameObject::IGameObject() {
	children = vector<IGameObject*>();
}

IGameObject::~IGameObject() {
	Destroy();
}

void IGameObject::Destroy() {
}

void IGameObject::addMeToChildren(IGameObject* child) {
	children.push_back(child);
}

void IGameObject::updateParent(IGameObject* parent) {
	//TODO
}

void IGameObject::deleteFromChildren(IGameObject* child) {
	for (int i = 0; i < children.size(); i++) {
		if (children[i] == child) {
			children.erase(children.begin() + i);
			break;
		}
	}
}

void IGameObject::addToSceneTree(IGameObject* parent, string name, vec3 position, quat orientation, vec3 scale, bool enabled)
{
	this->parent = parent;
	this->name = name;
	this->position = position;
	this->orientation = orientation;
	this->scale = scale;
	this->enabled = enabled;
	this->matricesValid = false;

	if (parent != nullptr) parent->addMeToChildren(this);
}

void IGameObject::updateMatrices() {
	updateLocalMatrices();
	for (auto child : children) {
		child->updateMatrices();
	}
}

void IGameObject::invalidateMatrices() {
	if (matricesValid) {
		matricesValid = false;
		for (auto child : children) {
			child->invalidateMatrices();
		}
	}
}

vec3 IGameObject::getPosition()
{
	return position;
}

quat IGameObject::getOrientation() {
	return orientation;
}

vec3 IGameObject::getScale() {
	return scale;
}

void IGameObject::setPosition(vec3 pos)
{
	this->position = pos;
	invalidateMatrices();
}

void IGameObject::setOrientation(quat orientation)
{
	this->orientation = orientation;
	invalidateMatrices();
}

void IGameObject::setScale(vec3 scale) {
	this->scale = scale;
	invalidateMatrices();
}

json IGameObject::GetJSON() {
	json j;
	j["Enabled"] = enabled;
	j["Position"] = { position.x, position.y, position.z };
	j["Oritentation"] = { orientation.x, orientation.y, orientation.z, orientation.w };
	j["Scale"] = { scale.x, scale.y, scale.z };
	return j;
}

void IGameObject::updateLocalMatrices() {
	if (matricesValid) return;
	if (!parent->matricesValid) {
		cout << "GameObject " << name << ": Cannot update matrices, parent's (" << parent->name <<") matrices are invalid." << endl;
	}

	toParentMatrix = mat4(0);
	toParentMatrix[0][0] = scale.x;
	toParentMatrix[1][1] = scale.y;
	toParentMatrix[2][2] = scale.z;
	toParentMatrix[3][3] = 1.0f;

	toParentMatrix = mat4_cast(orientation) * toParentMatrix;

	toParentMatrix[3][0] = position[0];
	toParentMatrix[3][1] = position[1];
	toParentMatrix[3][2] = position[2];

	toWorldMatrix = toParentMatrix * parent->toWorldMatrix;

	matricesValid = true;
}