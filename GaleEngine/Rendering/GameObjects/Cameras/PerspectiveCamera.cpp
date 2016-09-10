#include "PerspectiveCamera.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <iostream>

using namespace Rendering;
using namespace GameObjects;
using namespace Cameras;
using namespace glm;
using namespace std;
using json = nlohmann::json;

const glm::vec3 PerspectiveCamera::DEFAULT_EYE = vec3(0, 0, 0);
const glm::vec3 PerspectiveCamera::DEFAULT_TARGET = vec3(1, 0, 0);
const glm::vec3 PerspectiveCamera::DEFAULT_UP = vec3(0, 0, 1);
const glm::vec3 PerspectiveCamera::VERTICAL = vec3(0, 0, 1);

const float Pi = pi<float>();

PerspectiveCamera::~PerspectiveCamera() {
	Destroy();
}

void PerspectiveCamera::Destroy() {
	IGameObject::Destroy();
}

PerspectiveCamera::PerspectiveCamera(string name, float near, float far, float aspect, float fovy) : Camera(name) {
	this->nearClip = near;
	this->farClip = far;
	this->aspect = aspect;
	this->fovy = fovy;
	globalPos = DEFAULT_EYE;
	up = DEFAULT_UP;
	target = DEFAULT_TARGET;
}

mat4 PerspectiveCamera::GetProjMatrix() {
	mat4 proj(0);
	float f = 1.0f / tan(fovy / 2.0f);
	proj[0][0] = f / aspect;
	proj[1][1] = f;
	proj[2][2] = (farClip + nearClip) / (nearClip - farClip);
	proj[3][2] = (2.0f * farClip * nearClip) / (nearClip - farClip);
	proj[2][3] = -1.0f;
	return proj;
}

mat4 PerspectiveCamera::GetViewMatrix() {
	updateLocalMatrices();

	return inverse(toWorldMatrix);
}

//void PerspectiveCamera::updateFrame() {
//	if (!matricesValid) {
//		cout << "Perspective Camera " << name << ": Cannot update frame, matrices are invalid." << endl;
//		frameUpdated = false;
//	}
//	else {
//		if (frameUpdated) return;
//		globalPos = vec3(toWorldMatrix * vec4(position, 1));
//		negGaze = normalize(globalPos - target);
//		right = normalize(cross(VERTICAL, negGaze));
//		up = cross(negGaze, right);
//		frameUpdated = true;
//	}
//}

void PerspectiveCamera::updateLocalMatrices()
{
	mat4 worldToParent = inverse(parent->toWorldMatrix);
	vec3 pTarget = vec3(worldToParent * vec4(target, 1));
	vec3 pPosition = vec3(worldToParent * vec4(globalPos, 1));
	vec3 pNegGaze = normalize(pPosition - pTarget);
	vec3 pVERTICAL = vec3(worldToParent * vec4(VERTICAL, 0));
	vec3 pRight = normalize(cross(pVERTICAL, pNegGaze));
	vec3 pUp = cross(pNegGaze, pRight);

	mat4 targetMat = mat4(vec4(pRight, 0), vec4(pUp, 0), vec4(pNegGaze, 0), vec4(pPosition, 1));
	mat4 scaleMat = mat4(1);
	scaleMat[0][0] = scale.x;
	scaleMat[1][1] = scale.y;
	scaleMat[2][2] = scale.z;
	this->toParentMatrix = targetMat * scaleMat;
	this->toWorldMatrix = parent->toWorldMatrix * this->toParentMatrix;

	negGaze = vec3(parent->toWorldMatrix * vec4(pNegGaze, 0));
	right = vec3(parent->toWorldMatrix * vec4(pRight, 0));
	up = vec3(parent->toWorldMatrix * vec4(pUp, 0));

	setOrientation(quat(mat3(targetMat)));
	setPosition(pPosition);
	matricesValid = true;

	//negGaze = normalize(globalPos - target);
	//right = normalize(cross(VERTICAL, negGaze));
	//up = cross(negGaze, right);

	//this->toWorldMatrix = mat4(vec4(right, 0), vec4(up, 0), vec4(negGaze, 0), vec4(globalPos, 1));
	//this->toParentMatrix = inverse(parent->toWorldMatrix) * this->toWorldMatrix;

	//setOrientation(quat(mat3(toParentMatrix)));
	//setPosition(vec3(column(toParentMatrix, 3)));
	//matricesValid = true;
}

void PerspectiveCamera::LookAt(vec3 pos, vec3 target, vec3 up) {
	this->globalPos = pos;
	this->target = target;
	this->up = up;

	updateLocalMatrices();

	//frameUpdated = false;
}

void PerspectiveCamera::Orbit(float dPhi, float dTheta, float boundSize)
{
	//updateFrame();
	vec3 rho = normalize(globalPos - target);
	vec3 e = normalize(cross(VERTICAL, rho));

	float theta = acos(dot(rho, VERTICAL));
	float newTheta = theta + dTheta;
	if (newTheta <= 0) dTheta = 0.001f - theta;
	if (newTheta >= Pi) dTheta = Pi - 0.001f - theta;

	rho = rotate(rho, dTheta, e);
	rho = rotate(rho, dPhi, VERTICAL);
	rho = normalize(rho);

	globalPos = target + rho;

	updateLocalMatrices();
	//frameUpdated = false;
}

void PerspectiveCamera::Dolly(float dRho)
{
	//updateFrame();
	globalPos -= dRho * negGaze;
	target -= dRho * negGaze;

	updateLocalMatrices();
	//update local matrices
	//frameUpdated = false;
}

void PerspectiveCamera::Zoom(float zoomFactor)
{
	float tfovy = tan(fovy / 2.0f);
	tfovy *= zoomFactor;
	fovy = 2.0f * atan(tfovy);
	if (fovy > Pi) fovy = Pi - 0.001f;
	if (fovy <= 0) fovy = 0.001f;
}

void PerspectiveCamera::Strafe(float dx, float dy)
{
	//updateFrame();
	globalPos += right * dx + up * dy;
	target += right * dx + up * dy;
	updateLocalMatrices();
	//frameUpdated = false;
}

json PerspectiveCamera::GetSourceJSON() const {
	json j = IGameObject::GetSourceJSON();
	j["Type"] = "PerspectiveCamera";
	j["FoVy"] = fovy;
	j["Target"] = { target.x, target.y, target.z };
	j["Up"] = { up.x, up.y, up.z };
	j["Rigth"] = { right.x, right.y, right.z };
	j["NearClip"] = nearClip;
	j["FarClip"] = farClip;
	j["Aspect"] = aspect;
	return j;
}