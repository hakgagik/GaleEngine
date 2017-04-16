#include "GameObject.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace Rendering;
using namespace GameObjects;
using namespace std;
using namespace glm;
using json = nlohmann::json;

GameObject::GameObject(std::string name) {
	children = vector<GameObject*>();
	this->name = name;
}

GameObject::~GameObject() {
	Destroy();
}

void GameObject::Destroy() {
}

void GameObject::AddMeToChildren(GameObject* child) {
	children.push_back(child);
}

void GameObject::UpdateParent(GameObject* parent) {
	//TODO
}

void GameObject::DeleteFromChildren(GameObject* child) {
	for (int i = 0; i < children.size(); i++) {
		if (children[i] == child) {
			children.erase(children.begin() + i);
			break;
		}
	}
}

void GameObject::AddToSceneTree(GameObject* parent, vec3 &position, quat &orientation, vec3 &scale, bool enabled)
{
	this->parent = parent;
	this->position = position;
	this->orientation = orientation;
	this->scale = scale;
	this->enabled = enabled;
	this->matricesValid = false;
	parent->AddMeToChildren(this);
}

void GameObject::UpdateMatrices() {
	updateLocalMatrices();
	for (auto child : children) {
		child->UpdateMatrices();
	}
}

void GameObject::InvalidateMatrices() {
	if (matricesValid) {
		matricesValid = false;
		for (auto child : children) {
			child->InvalidateMatrices();
		}
	}
}

vec3 GameObject::getPosition() const
{
	return position;
}

quat GameObject::getOrientation() const {
	return orientation;
}

vec3 GameObject::getScale() const {
	return scale;
}

void GameObject::setPosition(vec3 pos)
{
	this->position = pos;
	InvalidateMatrices();
}

void GameObject::setOrientation(quat orientation)
{
	this->orientation = orientation;
	InvalidateMatrices();
}

void GameObject::setScale(vec3 scale) {
	this->scale = scale;
	InvalidateMatrices();
}

vector<string> GameObject::GetDebugOutput() {
	vector<string> debugStr(4);
	stringstream ss;
	ss << fixed << setprecision(2);

	ss << "GameObject " << name;
	debugStr[0] = ss.str();
	ss.str("");

	ss << "Pos: [" << position.x << ", " << position.y << ", " << position.x << "]";
	debugStr[1] = ss.str();
	ss.str("");

	ss << "Rot: [" << orientation.x << ", " << orientation.y << ", " << orientation.z << ", " << orientation.w << "]";
	debugStr[2] = ss.str();
	ss.str("");

	ss << "Sca: [" << scale.x << ", " << scale.y << ", " << scale.z << "]";
	debugStr[3] = ss.str();

	return debugStr;
}

json GameObject::GetSourceJSON() const {
	json j;
	return j;
}

json GameObject::GetSceneJSON() const {
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

void GameObject::updateLocalMatrices() {
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