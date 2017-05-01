#include "Light.h"

using namespace Rendering;
using namespace GameObjects;
using namespace Lights;
using namespace std;
using namespace glm;
using json = nlohmann::json;

Light::Light(string name, vec3 color, vec3 intensity, float32 cutoff) : GameObject(name) {
	this->Color = color;
	this->Intensity = intensity;
	this->Cutoff = cutoff;
}

Light::~Light() {}

void Light::Destroy() {
	GameObject::Destroy();
}

json Light::GetSourceJSON() const {
	json j = GameObject::GetSourceJSON();
	j["Type"] = "PointLight";
	j["Cutoff"] = Cutoff;
	j["Color"] = { Color.x, Color.y, Color.z };
	j["Attentuation"] = { Intensity.x, Intensity.y, Intensity.z };
	return j;
}