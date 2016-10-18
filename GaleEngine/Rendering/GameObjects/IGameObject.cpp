#include "IGameObject.h"
#include <iostream>

using namespace Rendering;
using namespace GameObjects;
using namespace std;
using namespace glm;
using json = nlohmann::json;

IGameObject::IGameObject(std::string name) {
	children = vector<IGameObject*>();
	this->name = name;
}

IGameObject::~IGameObject() {
	Destroy();
}

void IGameObject::Destroy() {
}

void IGameObject::AddMeToChildren(IGameObject* child) {
	children.push_back(child);
}

void IGameObject::UpdateParent(IGameObject* parent) {
	//TODO
}

void IGameObject::DeleteFromChildren(IGameObject* child) {
	for (int i = 0; i < children.size(); i++) {
		if (children[i] == child) {
			children.erase(children.begin() + i);
			break;
		}
	}
}

void IGameObject::AddToSceneTree(IGameObject* parent, vec3 &position, quat &orientation, vec3 &scale, bool enabled)
{
	this->parent = parent;
	this->position = position;
	this->orientation = orientation;
	this->scale = scale;
	this->enabled = enabled;
	this->matricesValid = false;
	parent->AddMeToChildren(this);
}

void IGameObject::UpdateMatrices() {
	updateLocalMatrices();
	for (auto child : children) {
		child->UpdateMatrices();
	}
}

void IGameObject::InvalidateMatrices() {
	if (matricesValid) {
		matricesValid = false;
		for (auto child : children) {
			child->InvalidateMatrices();
		}
	}
}

vec3 IGameObject::getPosition() const
{
	return position;
}

quat IGameObject::getOrientation() const {
	return orientation;
}

vec3 IGameObject::getScale() const {
	return scale;
}

void IGameObject::setPosition(vec3 pos)
{
	this->position = pos;
	InvalidateMatrices();
}

void IGameObject::setOrientation(quat orientation)
{
	this->orientation = orientation;
	InvalidateMatrices();
}

void IGameObject::setScale(vec3 scale) {
	this->scale = scale;
	InvalidateMatrices();
}

json IGameObject::GetSourceJSON() const {
	json j;
	return j;
}

json IGameObject::GetSceneJSON() const {
	json j;
	j["Enabled"] = enabled;
	j["Position"] = { position.x, position.y, position.z };
	j["Orientation"] = { orientation.x, orientation.y, orientation.z, orientation.w };
	j["Scale"] = { scale.x, scale.y, scale.z };
	for (auto child : children) {
		j["Children"][child->name] = child->GetSceneJSON();
	}
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
	toWorldMatrixInv = inverse(toWorldMatrix);

	matricesValid = true;
}