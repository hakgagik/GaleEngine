#include "Material_Manager.h"
#include "Texture_Manager.h"
#include "../Rendering/Materials/IMaterial.h"
#include "../Rendering/Materials/MaterialsHeader.h"
#include <glm/glm.hpp>

using namespace Rendering;
using namespace Materials;
using namespace Managers;
using namespace std;
using namespace glm;
using json = nlohmann::json;

Material_Manager Material_Manager::instance;

void Material_Manager::Init() { }

Material_Manager& Material_Manager::Get() {
	return instance;
}

Material_Manager::Material_Manager() {}

Material_Manager::~Material_Manager() {
	for (auto material : materialList) {
		delete material.second;
	}
}

void Material_Manager::DeleteMaterial(const string & materialName) {
	IMaterial* material = materialList[materialName];
	delete material;
	materialList.erase(materialName);
}

const IMaterial* Material_Manager::GetMaterial(const string & materialName) const {
	return materialList.at(materialName);
}

const void Material_Manager::AddMaterial() {
	// TODO
}

void Material_Manager::LoadFromJSON(json &j) {
	for (json::iterator it = j["Materials"].begin(); it != j["Materials"].end(); ++it) {
		json m = *it;
		if (m["Type"] == "SingleColor") {
			vec4 color(m["DiffuseColor"][0], m["DiffuseColor"][1], m["DiffuseColor"][2], m["DiffuseColor"][3]);
			materialList[it.key()] = new SingleColorMaterial(color);
		}
		else if (m["Type"] = "Lambertian") {
			vec4 diffuseColor(m["DiffuseColor"][0], m["DiffuseColor"][1], m["DiffuseColor"][2], m["DiffuseColor"][3]);
			const Texture* diffuseTexture = nullptr;
			if (!m["DiffuseTexture"].is_null()) {
				diffuseTexture = Texture_Manager::Get().GetTexture(m["DiffuseTexture"]);
			}
			materialList[it.key()] = new LambertianMaterial(diffuseColor, diffuseTexture);
		}
		else if (m["Type"] = "BlinnPhong") {
			vec4 diffuseColor(m["DiffuseColor"][0], m["DiffuseColor"][1], m["DiffuseColor"][2], m["DiffuseColor"][3]);
			vec4 specularColor(m["DiffuseColor"][0], m["DiffuseColor"][1], m["DiffuseColor"][2], m["DiffuseColor"][3]);
			const Texture* diffuseTexture = nullptr;
			const Texture* specularTexture = nullptr;
			const Texture* exponentTexture = nullptr;
			if (!m["DiffuseTexture"].is_null()) {
				diffuseTexture = Texture_Manager::Get().GetTexture(m["DiffuseTexture"]);
			}
			if (!m["SpecularTexture"].is_null()) {
				specularTexture = Texture_Manager::Get().GetTexture(m["SpecularTexture"]);
			}
			if (!m["ExponentTexture"].is_null()) {
				exponentTexture = Texture_Manager::Get().GetTexture(m["ExponentTexture"]);
			}
			materialList[it.key()] = new BlinnPhongMaterial(diffuseTexture, specularTexture, exponentTexture, diffuseColor, specularColor, m["Exponent"]);
		}
		else {
			cout << "Material_Manager: Unrecognized material type, """ << m["Type"] << """." << endl;
		}
	}
}

void Material_Manager::WriteToJSON(json &j) {
	for (auto kv : materialList) {
		if (SingleColorMaterial* singleColorMat = dynamic_cast<SingleColorMaterial*>(kv.second)) {
			j["Materials"][kv.first]["Type"] = "SingleColor";
			j["Materials"][kv.first]["DiffuseColor"] = { singleColorMat->diffuseColor.r, singleColorMat->diffuseColor.g, singleColorMat->diffuseColor.b, singleColorMat->diffuseColor.a };
		}
		else if (LambertianMaterial* lambertianMat = dynamic_cast<LambertianMaterial*>(kv.second)) {
			j["Materials"][kv.first]["Type"] = "Lambertian";
			j["Materials"][kv.first]["DiffuseColor"] = { lambertianMat->diffuseColor.r, lambertianMat->diffuseColor.g, lambertianMat->diffuseColor.b, lambertianMat->diffuseColor.a };
			if (lambertianMat->diffuseTexture != nullptr) {
				j["Materials"][kv.first]["DiffuseTexture"] = lambertianMat->diffuseTexture->name;
			}
		}
		else if (BlinnPhongMaterial* blinnPhongMat = dynamic_cast<BlinnPhongMaterial*>(kv.second)) {
			j["Materials"][kv.first]["Type"] = "BlinnPhong";
			j["Materials"][kv.first]["DiffuseColor"] = { blinnPhongMat->diffuseColor.r, blinnPhongMat->diffuseColor.g, blinnPhongMat->diffuseColor.b, blinnPhongMat->diffuseColor.a };
			j["Materials"][kv.first]["SpecularColor"] = { blinnPhongMat->specularColor.r, blinnPhongMat->specularColor.g, blinnPhongMat->specularColor.b, blinnPhongMat->specularColor.a };
			j["Materials"][kv.first]["Exponent"] = { blinnPhongMat->exponent };
			if (blinnPhongMat->diffuseTexture != nullptr) {
				j["Materials"][kv.first]["DiffuseTexture"] = blinnPhongMat->diffuseTexture->name;
			}
			if (blinnPhongMat->specularTexture != nullptr) {
				j["Materials"][kv.first]["SpecularTexture"] = blinnPhongMat->specularTexture->name;
			}
			if (blinnPhongMat->exponentTexture != nullptr) {
				j["Materials"][kv.first]["ExponentTexture"] = blinnPhongMat->exponentTexture->name;
			}
		}
		else {
			cout << "Material Manager: Can't serialize material: " << kv.first << ". Unknown material type." << endl;
		}
	}
}