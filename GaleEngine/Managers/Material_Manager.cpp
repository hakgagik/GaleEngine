#include "Material_Manager.h"
#include "../Rendering/Materials/IMaterial.h"
#include "../Rendering/Materials/MaterialsHeader.h"

using namespace Rendering;
using namespace Materials;
using namespace Managers;
using namespace std;
using json = nlohmann::json;

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