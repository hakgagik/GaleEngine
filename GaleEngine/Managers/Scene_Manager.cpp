// Modified from http://in2gpu.com/opengl-3/
#include "Scene_Manager.h"
#include "Shader_Manager.h"
#include "Model_Manager.h"
#include "Texture_Manager.h"
#include "Material_Manager.h"
#include "Input_Manager.h"
#include "Physics_Manager.h"
#include "../Rendering/IRenderer.h"
#include "../Rendering/ForwardRenderer.h"
#include "../Rendering/GameObjects/Cameras/PerspectiveCamera.h"
#include "../Rendering/GameObjects/Models/Model.h"
#include "../Rendering/GameObjects/Models/ModelClone.h"
#include "../Rendering/GameObjects/Lights/Light.h"
#include "../Rendering/GameObjects/HeadNode.h"
#include "../Rendering/Materials/MaterialsHeader.h"
#include "../Rendering/GameObjects/Models/Fragment.h"
#include "../lib/json.hpp"
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace Managers;
using namespace Rendering;
using namespace GameObjects;
using namespace Cameras;
using namespace Models;
using namespace Lights;
using namespace std;
using namespace glm;
using namespace nlohmann;

// TODO: should JSON strings be consts or defined?

Scene_Manager::Scene_Manager()
{
	glEnable(GL_DEPTH_TEST);
	
	shader_manager = new Shader_Manager();
	model_manager = new Model_Manager();
	texture_manager = new Texture_Manager();
	material_manager = new Material_Manager();
	physics_manager = new Physics_Manager();
	renderer = new ForwardRenderer();
	sceneInitialized = false;
}

Scene_Manager::~Scene_Manager() {
	delete shader_manager;
	delete model_manager;
	delete texture_manager;
	delete material_manager;
	delete physics_manager;
	delete renderer;
	delete headNode;
	for (Camera* cam : cameras) delete cam;
	for (Light* light : lights) delete light;
}

// Any setups before the actual drawing begins are placed here
void Scene_Manager::notifyBeginFrame() {
	//model_manager->Update();
	handleInputs();
	headNode->UpdateMatrices();
	renderer->setCamera(activeCam);
	renderer->setLights(lights);
}

// Anything with GL calls that draw to a framebuffer goes here
void Scene_Manager::notifyDisplayFrame() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	model_manager->Draw(renderer);
}

void Scene_Manager::notifyEndFrame() {
	// apparently nothing here either
}

void Scene_Manager::notifyReshape(int width, int height, int previous_width, int previous_height) {
	// and nothing here either D:
	glViewport(0, 0, width, height);
	activeCam->aspect = (float)width / (float)height;
}

void Scene_Manager::handleInputs()
{
	vec2 Strafe = vec2(0);
	if (Input_Manager::arrowDown) {
		Strafe += vec2(0, -1);
	}
	if (Input_Manager::arrowUp) {
		Strafe += vec2(0, 1);
	}
	if (Input_Manager::arrowLeft) {
		Strafe += vec2(-1, 0);
	}
	if (Input_Manager::arrowRight) {
		Strafe += vec2(1, 0);
	}
	if (Input_Manager::testKey) {
		activeCam->Orbit(0, 0.1f, 0.1f);
	}
	if (Input_Manager::arrowForward) {
		activeCam->Dolly(dStrafe);
	}
	if (Input_Manager::arrowBack) {
		activeCam->Dolly(-dStrafe);
	}
	if (Input_Manager::zoomForward) {
		activeCam->Zoom(1 / (1.0f + dStrafe));
	}
	if (Input_Manager::zoomBack) {
		activeCam->Zoom(1 + dStrafe);
	}
	Strafe *= dStrafe;
	activeCam->Strafe(Strafe.x, Strafe.y);
	if (Input_Manager::mouseLeftButton == 0) {
		Strafe.x = -Input_Manager::mouseDelta.x * dTheta;
		Strafe.y = -Input_Manager::mouseDelta.y * dTheta;
		activeCam->Orbit(Strafe.x, Strafe.y, dStrafe);
	}
	activeCam->InvalidateMatrices();
	Input_Manager::update();
}

void Scene_Manager::SetRenderer(IRenderer* renderer) {
	this->renderer = renderer;
}

// TODO::add a "json_utils.h"
void Scene_Manager::BuildSceneFromJSON(string &filename) {
	string jsonString = ReadFile(filename);
	json j = json::parse(jsonString);

	//Create shaders
	shader_manager->CreateProgram("single_color", j["Shaders"]["SingleColor"]["Vertex"], j["Shaders"]["SingleColor"]["Fragment"]);
	shader_manager->CreateProgram("lambertian", j["Shaders"]["Lambertian"]["Vertex"], j["Shaders"]["Lambertian"]["Fragment"]);
	
	//Load textures, materials, models, and physics
	texture_manager->LoadFromJSON(j);
	material_manager->LoadFromJSON(j);
	model_manager->LoadFromJSON(j);
	physics_manager->LoadFromJSON(j);

	//Container to temporarily hold all the IGameObjects, until we place them in the scene.
	unordered_map<string, IGameObject*> gameObjects;
	for (auto kv : model_manager->GetModelList()) {
		gameObjects[kv.first] = kv.second;
	}
	for (auto kv : model_manager->GetCloneList()) {
		gameObjects[kv.first] = kv.second;
	}

	//Load cameras
	for (json::iterator it = j["Cameras"].begin(); it != j["Cameras"].end(); ++it) {
		// TODO: abastract away camera creation to another manager
		json c = *it;
		if (c["Type"] == "PerspectiveCamera") {
			PerspectiveCamera* camera = new PerspectiveCamera(it.key(), (float)c["NearClip"], (float)c["FarClip"], (float)c["Aspect"], (float)c["FoVy"]);
			gameObjects[camera->name] = camera;
			cameras.push_back(camera);
		}
	}

	//Load lights
	for (json::iterator it = j["Lights"].begin(); it != j["Lights"].end(); ++it) {
		json l = *it;
		if (l["Type"] == "PointLight") {
			vec3 color(l["Light"][0], l["Light"][1], l["Light"][2]);
			vec3 attentuation(l["Attenuation"][0], l["Attenuation"][1], l["Attenuation"][2]);	
			Light* light = new Light(it.key(), color, attentuation, l["Cutoff"]);
			gameObjects[light->name] = light;
			lights.push_back(light);
		}
	}

	//Build scene tree
	activeCam = cameras[j["ActiveCam"]];
	headNode = new HeadNode("Head Node");
	buildSceneTreeBranch(headNode, nullptr, j["Head Node"], gameObjects);
}

void Scene_Manager::SaveSceneToJSON(const string &filename) {
	json j;
	j["Shaders"]["SingleColor"]["Vertex"] = "Shaders\\standard.vert";
	j["Shaders"]["SingleColor"]["Fragment"] = "Shaders\\single_color.frag";
	j["Shaders"]["Lambertian"]["Vertex"] = "Shaders\\standard.vert";
	j["Shaders"]["Lambertian"]["Fragment"] = "Shaders\\lambertian.frag";
	texture_manager->WriteToJSON(j);
	material_manager->WriteToJSON(j);
	model_manager->WriteToJSON(j);
	model_manager->WriteModelsToSourceJSON();
	physics_manager->WriteToJSON(j);
	for (Camera* camera : cameras) {
		j["Cameras"][camera->name] = camera->GetSourceJSON();
	}
	for (Light* light : lights) {
		j["Lights"][light->name] = light->GetSourceJSON();
	}
	//j["SceneTree"] = writeBranchToJSON(headNode);
	j["HeadNode"] = headNode->GetSceneJSON();
	j["ActiveCam"] = activeCam->name;
	ofstream output(filename);
	cout << "Printing to " << filename << endl;
	output << j.dump(4);
	output.close();
}

void Scene_Manager::SetupTestScene()
{
	//Create shaders
	shader_manager->CreateProgram("single_color", "Shaders\\standard.vert", "Shaders\\single_color.frag");
	shader_manager->CreateProgram("lambertian", "Shaders\\standard.vert", "Shaders\\lambertian.frag");

	//Define some generic transformations
	vec3 zero(0);
	vec3 one(1);
	quat norot = angleAxis(0.0f, zero);
	quat aLittleRot = angleAxis(0.4f, vec3(1, 0, 0));
	quat aLittelMoreRot = angleAxis(0.4f, vec3(1));

	//Create the head node
	headNode = new HeadNode("Head Node");
	headNode->AddToSceneTree(nullptr, zero, norot, one, false);
	
	//Create lights
	lights.push_back(new Light("Main Light"));
	lights.push_back(new Light("Second Light"));
	lights[0]->AddToSceneTree(headNode, vec3(0, 0, 2), norot, one);
	lights[1]->AddToSceneTree(headNode, vec3(5), norot, one);
	
	// Make a square. It's called triangle.
	vector<VertexFormat> vertices = { VertexFormat(vec4(0, 1, 0, 1), vec3(0, -1, 0), vec2(0, 0), vec4(1, 0, 0, 1)),
		VertexFormat(vec4(1, 1, 0, 1), vec3(0, -1, 0), vec2(1, 0), vec4(1, 0, 0, 1)),
		VertexFormat(vec4(1, 1, 1, 1), vec3(0, -1, 0), vec2(1, 1), vec4(1, 0, 0, 1)),
		VertexFormat(vec4(0, 1, 1, 1), vec3(0, -1, 0), vec2(0, 1), vec4(1, 0, 0, 1)), };
	vector<unsigned int> indices{ 0, 1, 2, 0, 2, 3 };
	Model* triangle = model_manager->CreateAndAdd("Triangle", vertices, indices);
	triangle->source = "ModelSources\\Triangle.json";
	triangle->AddToSceneTree(headNode, vec3(1.0f), -aLittleRot, one);
	Texture* tex = texture_manager->LoadandAddTexture("Images\\test.bmp");
	dynamic_cast<Materials::LambertianMaterial*>(triangle->GetFragmentMat("Main"))->diffuseTexture = tex;

	// Get a sphere copy
	Model* sphere = model_manager->PromoteToModel(model_manager->getSphereCopy("Sphere"));
	sphere->AddToSceneTree(headNode, vec3(2.0f), aLittleRot, one);
	sphere->SetFragmentMat("Main", triangle->GetFragmentMat("Main"));
	//Model* sphere2 = model_manager->getSphereCopy("SphereTwo");
	//sphere2->addToSceneTree(headNode, sphere->name, vec3(0.0f, 3.0f, 1.0f), norot, one);

	// Get a cube copy
	Model* cube = model_manager->PromoteToModel(model_manager->getCubeCopy("Cube"));
	cube->AddToSceneTree(headNode, vec3(3, 2, 3), aLittelMoreRot, one);
	Texture* cubeTex = texture_manager->LoadandAddTexture("Images\\cubeTest.bmp");
	cube->SetFragmentMat("Main", new Materials::LambertianMaterial(vec4(1.0f), cubeTex));

	// Create the main camera
	activeCam = new PerspectiveCamera("Main Camera");
	activeCam->AddToSceneTree(headNode, zero, norot, one);
	activeCam->LookAt(vec3(0.5f, 0, 0.5f), vec3(0.5f, 1, 0.5f), vec3(0, 0, 1));
	cameras.push_back(activeCam);

	// Spam some sphere clones
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			string name = "Sphere" + to_string(10 * i + j);
			ModelClone* sphere = model_manager->getSphereCopy(name);
			sphere->AddToSceneTree(headNode, vec3(i, j, 0), norot, one * 0.5f);
		}
	}

	sceneInitialized = true;
	Input_Manager::registerCallbacks();
	headNode->UpdateMatrices();
	SaveSceneToJSON("JSON\\testScene.json");
}

void Scene_Manager::buildSceneTreeBranch(IGameObject* node, IGameObject* parent, json branch, unordered_map<string, IGameObject*> &gameObjects) {
	vec3 position(branch["Position"][0], branch["Position"][1], branch["Position"][2]);
	quat orientation(branch["Orientation"][0], branch["Orientation"][1], branch["Orientation"][2], branch["Orientation"][3]);
	vec3 scale(branch["Scale"][0], branch["Scale"][1], branch["Scale"][2]);
	node->AddToSceneTree(parent, position, orientation, scale, branch["Enabled"]);
	for (json::iterator it = branch["Children"].begin(); it != branch["Children"].end(); it++) {
		buildSceneTreeBranch(gameObjects[it.key()], node, *it, gameObjects);
	}
}

string Scene_Manager::ReadFile(const string &filename) {
	ifstream file(filename);
	string output;
	if (!file.good()) {
		cout << "Can't read file " << filename.c_str() << endl;
		terminate();
	}

	file.seekg(0, ios::end);
	output.resize((unsigned int)file.tellg());
	file.seekg(0, ios::beg);
	file.read(&output[0], output.size());
	file.close();
	return output;
}

//json Scene_Manager::writeBranchToJSON(IGameObject* node) {
//	json j;
//	for (IGameObject* childNode : node->children) {
//		j[childNode->name] = writeBranchToJSON(childNode);
//	}
//	return j;
//}