// Modified from http://in2gpu.com/opengl-3/
#include "Model_Manager.h"
#include "../Rendering/GameObjects/Models/Model.h"
#include "../Rendering/GameObjects/Models/ModelClone.h"
#include "../Rendering/IRenderer.h"
#include "../Rendering/VertexFormat.h"
#include <unordered_set>
#include <fstream>

using namespace Managers;
using namespace Rendering;
using namespace GameObjects;
using namespace Models;
using namespace glm;
using namespace std;
using json = nlohmann::json;

const float Pi = glm::pi<float>();
const string SPHERE_TEMPLATE_SOURCE = "JSON\\ModelSources\\SphereTemplate.json";
const string CUBE_TEMPLATE_SOURCE = "JSON\\ModelSources\\CubeTemplate.json";
const string RECT_TEMPLATE_SOURCE = "JSON\\ModelSources\\RectTemplate.json";

Model_Manager::Model_Manager() {
}

Model_Manager::~Model_Manager() {
	for (auto model : modelList) {
		delete model.second;
	}
	for (auto clone : cloneList) {
		delete clone.second;
	}
}

void Model_Manager::DeleteModel(const string& gameModelName) {
	Model* model = modelList[gameModelName];
	for (auto clone : cloneList) {
		if (clone.second->source == model) {
			clone.second->Destroy();
			cloneList.erase(clone.second->name);
		}
	}
	model->Destroy();
	modelList.erase(gameModelName);
}

void Model_Manager::DeleteClone(const string& cloneName) {
	ModelClone* clone = cloneList[cloneName];
	clone->Destroy();
	cloneList.erase(cloneName);
}

Model* Model_Manager::CreateAndAdd(string name, vector<VertexFormat> &verts, vector<unsigned int> &indices, vec4 color)
{
	Model* model = new Model(name, verts, indices, color);
	modelList[name] = model;
	return model;
}

ModelClone* Model_Manager::CloneAndAddd(string name, Model* source) {
	ModelClone* clone = new ModelClone(source, name);
	cloneList[name] = clone;
	return clone;
}

void Model_Manager::createSphereTemplate(int thetaDiv, int phiDiv) {
	vector<VertexFormat> verts;
	vector<unsigned int> indices;
	float theta, phi, st, ct, sp, cp;
	vec4 x = vec4(1), t = vec4(1);
	t[3] = 0;
	vec2 u;
	//ofstream output("out.dat");
	
	for (int i = 0; i <= thetaDiv; i++) {
		theta = (float)i / (float)thetaDiv * Pi;
		u.y = 1.0f - theta/Pi;
		st = sin(theta);
		ct = cos(theta);
		x.z = ct;
		t.z = -st;
		for (int j = 0; j <= phiDiv; j++) {
			u.x = (float)j / (float)phiDiv;
			phi = u.x * 2.0f * Pi;
			sp = sin(phi);
			cp = cos(phi);
			x.x = st*cp;
			x.y = st*sp;
			t.x = ct*cp;
			t.y = ct*sp;
			verts.push_back(VertexFormat(x, vec3(x), u, t));
			//output << x.x << '\t' << x.y << '\t' << x.z << endl;
		}
	}
	for (int i = 0; i < thetaDiv; i++) {
		for (int j = 0; j < phiDiv; j++) {
			int index = i * (phiDiv + 1) + j;
			indices.push_back(index);
			indices.push_back(index + (phiDiv + 1));
			indices.push_back(index + (phiDiv + 1) + 1);
			indices.push_back(index);
			indices.push_back(index + (phiDiv + 1) + 1);
			indices.push_back(index + 1);
		}
	}

	//for (int i = 0; i < indices.size(); i++) {
	//	output << indices[i] << endl;
	//}

	sphereTemplate = new Model("Template Sphere", verts, indices);
	sphereTemplate->source = SPHERE_TEMPLATE_SOURCE;
}

void Model_Manager::createCubeTemplate() {
	// Implement this method
	vector<VertexFormat> verts;

	vec4 v0(0, 0, 0, 1);
	vec4 v1(1, 0, 0, 1);
	vec4 v2(0, 1, 0, 1);
	vec4 v3(1, 1, 0, 1);
	vec4 v4(0, 0, 1, 1);
	vec4 v5(1, 0, 1, 1);
	vec4 v6(0, 1, 1, 1);
	vec4 v7(1, 1, 1, 1);

	vec3 n0(0, -1, 0);
	vec3 n1(1, 0, 0); 
	vec3 n2(0, 1, 0);
	vec3 n3(-1, 0, 0);
	vec3 n4(0, 0, -1);
	vec3 n5(0, 0, 1);

	vec2 uv0(0, 0);
	vec2 uv1(0.25, 0);
	vec2 uv2(0.5, 0);
	vec2 uv3(0.75, 0);
	vec2 uv4(0, 0.25);
	vec2 uv5(0.25, 0.25);
	vec2 uv6(0.5, 0.25);
	vec2 uv7(0.75, 0.25);
	vec2 uv8(0.25, 0.5);
	vec2 uv9(0.5, 0.5);
	vec2 uv10(0.25, 0.75);
	vec2 uv11(0.5, 0.75);
	vec2 uv12(0.25, 1);
	vec2 uv13(0.5, 1);

	vec4 t0(n1, 0);
	vec4 t1(n2, 0);
	vec4 t2(n3, 0);
	vec4 t3(n0, 0);

	//-y (0-3)
	verts.push_back(VertexFormat(v0, n0, uv0, t0));
	verts.push_back(VertexFormat(v1, n0, uv1, t0));
	verts.push_back(VertexFormat(v5, n0, uv5, t0));
	verts.push_back(VertexFormat(v4, n0, uv4, t0));
	
	//+x (4-7)
	verts.push_back(VertexFormat(v1, n1, uv1, t1));
	verts.push_back(VertexFormat(v3, n1, uv2, t1));
	verts.push_back(VertexFormat(v7, n1, uv6, t1));
	verts.push_back(VertexFormat(v5, n1, uv5, t1));

	//+y (8-11)
	verts.push_back(VertexFormat(v3, n2, uv2, t2));
	verts.push_back(VertexFormat(v2, n2, uv3, t2));
	verts.push_back(VertexFormat(v6, n2, uv7, t2));
	verts.push_back(VertexFormat(v7, n2, uv6, t2));

	//-x (12-15)
	verts.push_back(VertexFormat(v2, n3, uv11, t3));
	verts.push_back(VertexFormat(v0, n3, uv10, t3));
	verts.push_back(VertexFormat(v4, n3, uv8, t3));
	verts.push_back(VertexFormat(v6, n3, uv9, t3));

	//-z (16-19)
	verts.push_back(VertexFormat(v0, n4, uv10, t2));
	verts.push_back(VertexFormat(v1, n4, uv12, t2));
	verts.push_back(VertexFormat(v3, n4, uv13, t2));
	verts.push_back(VertexFormat(v2, n4, uv11, t2));

	//+z (20-23)
	verts.push_back(VertexFormat(v4, n5, uv8, t0));
	verts.push_back(VertexFormat(v5, n5, uv5, t0));
	verts.push_back(VertexFormat(v7, n5, uv6, t0));
	verts.push_back(VertexFormat(v6, n5, uv9, t0));

	vector<unsigned int> indices = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23
	};

	cubeTemplate = new Model("Cube Template", verts, indices);
	cubeTemplate->source = CUBE_TEMPLATE_SOURCE;
}

void Model_Manager::createRectTemplate() {
	// Implement this method
	rectTemplate = new Model("Rectangle Template");
	rectTemplate->source = RECT_TEMPLATE_SOURCE;
}

ModelClone* Model_Manager::getSphereCopy(string name) {
	if (sphereTemplate == nullptr) {
		createSphereTemplate();
	}
	ModelClone* sphere = new ModelClone(sphereTemplate, name);
	cloneList[name] = sphere;
	return sphere;
}

ModelClone* Model_Manager::getCubeCopy(string name)
{
	if (cubeTemplate == nullptr) {
		createCubeTemplate();
	}
	ModelClone* cube = new ModelClone(cubeTemplate, name);
	cloneList[name] = cube;
	return cube;
}

void Model_Manager::Draw(IRenderer * renderer)
{

	for (auto model : modelList) {
		if (model.second->enabled) {
			renderer->Render(model.second);
		}
	}

	for (auto clone : cloneList) {
		if (clone.second->enabled) {
			renderer->Render(clone.second);
		}
	}
}

void Model_Manager::Update() {
	for (auto model : modelList) {
		//model.second->UpdateMatrices();
	}
}

const Model * Model_Manager::GetModel(const string& gameModelName) const {
	return modelList.at(gameModelName);
}

const ModelClone* Model_Manager::GetClone(const string& cloneName) const {
	return cloneList.at(cloneName);
}

void Model_Manager::BuildAndAddFromOBJ(const std::string& filename) {

}

Model* Model_Manager::PromoteToModel(ModelClone* clone) {
	Model* model = new Model(clone->source, clone->name);
	modelList[model->name] = model;
	clone->Destroy();
	cloneList.erase(clone->name);
	return model;
}

void Model_Manager::LoadFromJSON(json &j) {

}

void Model_Manager::WriteToJSON(json &j) {
	for (auto kv : modelList) {
		j["Models"][kv.first] = kv.second->source;
	}
	for (auto kv : cloneList) {
		j["Clones"][kv.first] = kv.second->source->name;
	}
}

void Model_Manager::WriteModelsToSourceJSON() {
	cout << "Serializing Models" << endl;

	unordered_set<string> sources;
	
	if (sphereTemplate != nullptr) {
		cout << "Currently Serializing: Sphere Template\t" << endl;
		json j = sphereTemplate->GetSourceJSON();
		sources.emplace(sphereTemplate->source);
		ofstream output(sphereTemplate->source);
		output << j.dump();
		output.close();
	}

	if (cubeTemplate != nullptr) {
		cout << "Currently Serializing: Cube Template\t" << endl;
		json j = cubeTemplate->GetSourceJSON();
		sources.emplace(cubeTemplate->source);
		ofstream output(cubeTemplate->source);
		output << j.dump();
		output.close();
	}

	if (rectTemplate != nullptr) {
		cout << "Currently Serializing: Rectangle Template\t" << endl;
		json j = rectTemplate->GetSourceJSON();
		sources.emplace(rectTemplate->source);
		ofstream output(rectTemplate->source);
		output << j.dump();
		output.close();
	}

	for (auto kv : modelList) {
		if (!sources.count(kv.second->source)) {
			cout << "Currently Serializing:\t" << kv.first << endl;
			json j = kv.second->GetSourceJSON();
			sources.emplace(kv.second->source);
			ofstream output(kv.second->source);
			output << j.dump();
			output.close();
		}
	}

	cout << "Finished Serializing Models" << endl;
}

vector<string> Model_Manager::GetModelNames() {
	vector<string> names(modelList.size() + cloneList.size());
	return names;
}

unordered_map<string, Model*> Model_Manager::GetModelList() {
	return modelList;
}

unordered_map<string, ModelClone*> Model_Manager::GetCloneList() {
	return cloneList;
}