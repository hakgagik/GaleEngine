// Modified from http://in2gpu.com/opengl-3/
#include "Scene_Manager.h"
using namespace Managers;

Scene_Manager::Scene_Manager()
{
	glEnable(GL_DEPTH_TEST);
	
	shader_manager = new Shader_Manager();
	shader_manager->CreateProgram("colorShader", "Shaders\\Vertex_Shader.glsl", "Shaders\\Fragment_Shader.glsl");
	model_manager = new Model_Manager();
}

Scene_Manager::~Scene_Manager() {
	delete shader_manager;
	delete model_manager;
}

void Scene_Manager::notifyBeginFrame() {
	model_manager->Update();
}

void Scene_Manager::notifyDisplayFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	model_manager->Draw();
}

void Scene_Manager::notifyEndFrame() {
	// apparently nothing here either
}

void Scene_Manager::notifyReshape(int width, int height, int previous_width, int previous_height) {
	// and nothing here either D:
}