#include "UIModel.h"
#include <iostream>

using namespace UI;
using namespace std;
using namespace glm;

mat3 getRotationMatrix(float &orienation);

UIModel::UIModel(std::string name) {
	this->name = name;
}

UIModel::~UIModel() {
	Destroy();
}

void UIModel::Destroy() {
}

void UIModel::AddMeToChildren(UIModel* child) {
	children.push_back(child);
}

void UIModel::UpdateParent(UIModel* parent) {
	//TODO
}

void UIModel::DeleteFromChildren(UIModel* child) {
	for (int i = 0; i < children.size(); i++) {
		if (children[i] == child) {
			children.erase(children.begin() + i);
			break;
		}
	}
}

void UIModel::AddToSceneTree(UIModel* parent, vec2 &position, float &orientation, vec2 &scale, bool enabled) {
	this->parent = parent;
	this->position = position;
	this->orientation = orientation;
	this->scale = scale;
	this->enabled = enabled;
	this->matricesValid = false;
	parent->AddMeToChildren(this);
}

void UIModel::UpdateMatrices() {
	updateLocalMatrices();
	for (auto child : children) {
		child->UpdateMatrices();
	}
}

void UIModel::InvalidateMatrices() {
	if (matricesValid) {
		matricesValid = false;
		for (auto child : children) {
			child->InvalidateMatrices();
		}
	}
}

vec2 UIModel::getPosition() const {
	return position;
}

float UIModel::getOrientation() const {
	return orientation;
}

vec2 UIModel::getScale() const {
	return scale;
}

int UIModel::getDepth() const {
	return depth;
}

void UIModel::setPosition(vec2 &pos) {
	this->position = pos;
	InvalidateMatrices();
}

void UIModel::setOrientation(float &orientation) {
	this->orientation = orientation;
	InvalidateMatrices();
}

void UIModel::setScale(vec2 &scale) {
	this->scale = scale;
}


void UIModel::updateLocalMatrices() {
	if (matricesValid) return;
	if (!parent->matricesValid) {
		cout << "UIModel " << name << ": Cannot update matrices, parent's (" << parent->name << ") matrices are invalid." << endl;
	}
	
	depth = parent->depth + 1;

	toParentMatrix = mat3(0);
	toParentMatrix[0][0] = scale.x;
	toParentMatrix[1][1] = scale.y;
	toParentMatrix[2][2] = 1;
	
	toParentMatrix = getRotationMatrix(orientation) * toParentMatrix;
	
	toParentMatrix[2][0] = position[0];
	toParentMatrix[2][1] = position[1];

	toWorldMatrix = toParentMatrix * parent->toWorldMatrix;
	toWorldMatrixInv = inverse(toWorldMatrix);

	matricesValid = true;
}

mat3 getRotationMatrix(float &orienation)
{
	mat3 rot(0);
	rot[0][0] = rot[1][1] = cos(orienation);
	rot[0][1] = -sin(orienation);
	rot[1][0] = -rot[0][1];
	rot[2][2] = 1;
	return rot;
}
