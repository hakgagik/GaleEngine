#include "Model.h"
#include "../../Materials/SingleColorMaterial.h"
#include "../../Materials/LambertianMaterial.h"
#include "Fragment.h"
#include <glm/gtc/constants.hpp>
#include <iostream>

using namespace Rendering;
using namespace GameObjects;
using namespace Models;
using namespace Materials;
using namespace std;
using namespace glm;
using json = nlohmann::json;

Model::Model(string name) : IGameObject(name) {}

Model::Model(const Model* other, string name) : IGameObject(name) {
	cout << "Model: Copying " << other->name << " into " << name << ". This is very resource intensive. Should this be a clone instead?" << endl;
	this->position = other->position;
	this->orientation = other->orientation;
	this->scale = other->scale;
	this->source = other->source;
	this->verts = other->verts;
	this->indices = other->indices;
	for (auto kv : other->fragments) {
		fragments[kv.first] = kv.second;
	}

	GenerateVAO();
	UpdateVBO(true);
	UpdateIBO(true);
}

Model::Model(string name, vector<VertexFormat> &verts, vector<unsigned int> &indices, vec4 color) : IGameObject(name)
{
	this->verts = verts;
	this->indices = indices;

	GenerateVAO();
	UpdateVBO(true);
	UpdateIBO(true);

	//SingleColorMaterial* mat = new SingleColorMaterial(color);
	LambertianMaterial* mat = new LambertianMaterial(color);
	this->fragments["Main"] = new Fragment(this, mat, 0, indices.size(), 0, verts.size());
}

Model::~Model() {
	Destroy();
}

void Model::Update() {

}

void Model::UpdateVBO(bool force) {
	if (vboInvalid || force) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(VertexFormat), &verts[0], GL_STATIC_DRAW);
		vboInvalid = false;
	}
}

void Model::UpdateIBO(bool force) {
	if (iboInvalid || force) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}
}

GLuint Model::GetVao() const {
	return vao;
}

void Model::GenerateVAO() {
	GLuint vao;
	GLuint vbo;
	GLuint ibo;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, VertexFormat::position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, VertexFormat::normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, VertexFormat::uv));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, VertexFormat::tangent));

	glBindVertexArray(0);
	this->vao = vao;
	this->vbos.push_back(vbo);
	this->vbos.push_back(ibo);
}

const std::vector<GLuint>& Model::GetVbos() const {
	return vbos;
}

void Model::InvalidateVBO()
{
	vboInvalid = true;
}

void Model::Destroy() {
	IGameObject::Destroy();
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers((GLsizei)vbos.size(), &vbos[0]);
	vbos.clear();
}

void Model::UpdatePosition(int ind, vec4 &pos) {
	verts[ind].position = pos;
}

void Model::UpdatePosition(int ind, vec3 &pos) {
	verts[ind].position = vec4(pos, 1);
}

void Model::UpdateNormal(int ind, vec3 &norm) {
	verts[ind].normal = norm;
}

void Model::UpdateUV(int ind, vec2 &uv) {
	verts[ind].uv = uv;
}

void Model::UpdateTangent(int ind, vec4 &tan) {
	verts[ind].tangent = tan;
}

void Model::UpdateTangent(int ind, vec3 &tan) {
	verts[ind].tangent = vec4(tan, 0);
}

void Model::RecalculateNormals() {
	int numTris = indices.size() / 3;
	int numVerts = verts.size();
	vector<vec3> normals(numVerts);
	vector<float> adjacentTris(numVerts);

	for (int i = 0; i < numVerts; i++) {
		normals[i] = vec3(0);
		adjacentTris[i] = 0;
	}
	
	vec3 p0, p1, p2, n;
	int triInd, i0, i1, i2;
	for (int i = 0; i < numTris; i++) {
		triInd = i * 3;
		i0 = indices[triInd];
		i1 = indices[triInd + 1];
		i2 = indices[triInd + 2];
		p0 = vec3(verts[indices[triInd]].position);
		p1 = vec3(verts[indices[triInd + 1]].position);
		p2 = vec3(verts[indices[triInd + 2]].position);

		n = cross(p1 - p0, p2 - p0);

		normals[i0] += n;
		normals[i1] += n;
		normals[i2] += n;
		adjacentTris[i]++;
	}

	for (int i = 0; i < numVerts; i++) {
		if (adjacentTris[i] != 0) normals[i] /= adjacentTris[i];
		verts[i].normal = normals[i];
	}
	calculatedNormals = true;
}

void Model::RecalculateTangents() {
	// TODO: calculate tangents
	
}

const Fragment* Model::GetFragment(string fragName) const {
	return fragments.at(fragName);
}

vector<string> Model::GetFragNames() const {
	vector<string> fragNames;
	for (auto kv : fragments) {
		fragNames.push_back(kv.first);
	}
	return fragNames;
}

void Model::SetFragmentMat(string fragName, IMaterial* mat) {
	fragments[fragName]->material = mat;
}

IMaterial* Model::GetFragmentMat(string fragName) {
	return fragments[fragName]->material;
}

void Model::DeleteFragment(string fragName) {
	fragments.erase(fragName);
}

json Model::GetSourceJSON() const {
	json j = IGameObject::GetSourceJSON();

	for (int i = 0; i < verts.size(); i++) {
		j["Positions"][i] = { verts[i].position.x, verts[i].position.y, verts[i].position.z };
		j["UVs"][i] = { verts[i].uv.x, verts[i].uv.y };
		if (!calculatedNormals) j["Normals"][i] = { verts[i].normal.x, verts[i].normal.y , verts[i].normal.z };
		if (!calculatedTangents) j["Tangents"][i] = { verts[i].tangent.x, verts[i].tangent.y, verts[i].tangent.z };
	}

	for (int i : indices) {
		j["Indices"].push_back(i);
	}

	for (auto kv : fragments) {
		j["Fragments"][kv.first]["PrimitiveType"] = kv.second->primitiveType;
		j["Fragments"][kv.first]["IndexStartPointer"] = kv.second->indexStartPointer;
		j["Fragments"][kv.first]["IndexCount"] = kv.second->indexCount;
		j["Fragments"][kv.first]["Material"] = kv.second->material->name;
	}
	return j;
}

void Model::PrintVBO() const {
	glBindVertexArray(GetVao());
	float* vboPtr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	for (int i = 0; i < verts.size(); i++) {
		cout << "Pos:\t" << *(vboPtr + i) << "\t" << *(vboPtr + i + 1) << "\t" << *(vboPtr + i + 2) << "\t" << *(vboPtr + i + 3) << endl;
		cout << "Norm:\t" << *(vboPtr + i + 4) << "\t" << *(vboPtr + i + 5) << "\t" << *(vboPtr + i + 6) << endl;
		cout << "UV:\t" << *(vboPtr + i + 7) << "\t" << *(vboPtr + i + 8) << "\t" << endl;
		cout << "Tan:\t" << *(vboPtr + i + 9) << "\t" << *(vboPtr + i + 10) << "\t" << *(vboPtr + i + 11) << "\t" << *(vboPtr + i + 12) << endl;
		cout << endl;
	}
}
