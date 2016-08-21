#include "Light.h"

using namespace Rendering;
using namespace GameObjects;
using namespace Lights;
using namespace std;
using namespace glm;
using json = nlohmann::json;

Light::Light(string name, vec3 color, vec3 attenuation, float32 cutoff) : IGameObject(name) {
	this->color = color;
	this->attenuation = attenuation;
	this->cutoff = cutoff;
}

Light::~Light() {}

void Light::Destroy() {
	IGameObject::Destroy();
}

json Light::GetJSON() {
	json j = IGameObject::GetJSON();
	j["Type"] = "Light";
	j["Cutoff"] = cutoff;
	j["Color"] = { color.x, color.y, color.z };
	j["Attentuation"] = { attenuation.x, attenuation.y, attenuation.z };
	return j;
}