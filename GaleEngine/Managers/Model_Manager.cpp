// Modified from http://in2gpu.com/opengl-3/
#include "Model_Manager.h"
#include "../Rendering/GameObjects/Models/Model.h"
#include "../Rendering/IRenderer.h"
#include "../Rendering/VertexFormat.h"
//#include <fstream>

using namespace Managers;
using namespace Rendering;
using namespace GameObjects;
using namespace Models;
using namespace glm;
using namespace std;
using json = nlohmann::json;

const float Pi = glm::pi<float>();

Model_Manager::Model_Manager() {
}

Model_Manager::~Model_Manager() {
	for (auto model : modelList) {
		delete model.second;
	}
}

void Model_Manager::DeleteModel(const string& gameModelName) {
	Model* model = modelList[gameModelName];
	model->Destroy();
	modelList.erase(gameModelName);
}

void Model_Manager::AddModel(Model * model)
{
	modelList[model->name] = model;
}

Model* Model_Manager::CreateAndAdd(string name, vector<VertexFormat> &verts, vector<unsigned int> &indices, vec4 color)
{
	Model* model = new Model(verts, indices, color);
	model->name = name;
	AddModel(model);
	return model;
}

Model* Model_Manager::createSphere(int thetaDiv, int phiDiv) {
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

	return new Model(verts, indices);
}

Model* Model_Manager::createCube() {
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

	return new Model(verts, indices);
}

Model* Model_Manager::createRect() {
	// Implement this method
	return new Model();
}

Model* Model_Manager::getSphereCopy(string name) {
	if (sphereTemplate == nullptr) {
		sphereTemplate = createSphere();
	}
	Model* sphere = new Model(sphereTemplate, name);
	this->AddModel(sphere);
	return sphere;
}

Model* Model_Manager::getCubeCopy(string name)
{
	if (cubeTemplate == nullptr) {
		cubeTemplate = createCube();
	}
	Model* cube = new Model(cubeTemplate, name);
	this->AddModel(cube);
	return cube;
}

void Model_Manager::Draw(IRenderer * renderer)
{

	for (auto model : modelList) {
		if (model.second->enabled) {
			renderer->Render(model.second);
		}
	}
}

const Model * Model_Manager::GetModel(const string& gameModelName) const {
	return modelList.at(gameModelName);
}

void Model_Manager::BuildAndAddFromOBJ(const std::string& filename) {

}

void Model_Manager::Update() {
	for (auto model : modelList) {
		//model.second->updateMatrices();
	}
}

void Model_Manager::LoadFromJSON(json &j) {

}

void Model_Manager::WriteToJSON(json &j) {
	cout << "Serializing Models" << endl;
	for (auto kv : modelList) {
		j[kv.first] = kv.second->GetJSON();
		cout << "\rCurrently Serializing:\t" << kv.first;
	}
	cout << "\nFinished Serializing Models" << endl;
}