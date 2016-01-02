// Modified from http://in2gpu.com/opengl-3/
#include "Model_Manager.h"

using namespace Managers;
using namespace Rendering;

Model_Manager::Model_Manager() {
	Models::Triangle* triangle = new Models::Triangle();
	triangle->SetProgram(Shader_Manager::GetShader("colorShader"));
	triangle->Create();
	gameModelList["triangle"] = triangle;

	Models::Quad* quad = new Models::Quad();
	quad->SetProgram(Shader_Manager::GetShader("colorShader"));
	quad->Create();
	gameModelList["quad"] = quad;
}

Model_Manager::~Model_Manager() {
	for (auto model : gameModelList) {
		delete model.second;
	}
}

void Model_Manager::DeleteModel(const std::string& gameModelName) {
	IGameObject* model = gameModelList[gameModelName];
	model->Destroy();
	gameModelList.erase(gameModelName);
}

const IGameObject& Model_Manager::GetModel(const std::string& gameModelName) const {
	return (*gameModelList.at(gameModelName));
}

void Model_Manager::Update() {
	for (auto model : gameModelList) {
		model.second->Update();
	}
}

void Model_Manager::Draw() {
	for (auto model : gameModelList) {
		model.second->Draw();
	}
}