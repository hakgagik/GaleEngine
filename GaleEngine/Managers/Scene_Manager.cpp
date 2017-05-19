// Modified from http://in2gpu.com/opengl-3/
#include "Scene_Manager.h"
#include "Shader_Manager.h"
#include "Model_Manager.h"
#include "Texture_Manager.h"
#include "Material_Manager.h"
#include "UI_Manager.h"
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
#include "../Physics/Forces/Force.h"
#include "../lib/json.hpp"
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


//REMOVE LATER
#include "../Physics/PhysicsObjects/Solids/Cloth.h"
#include "../Physics/PhysicsObjects/Fluids/Fluid.h"
#include "../Physics/Forces/ConstantForce.h"
#include "../Physics/Forces/BoundingForce.h"
#include "GL/freeglut.h"
using Cloth = Physics::PhysicsObjects::Solids::Cloth;
using Fluid = Physics::PhysicsObjects::Fluids::Fluid;

using namespace Managers;
using namespace Rendering;
using namespace GameObjects;
using namespace Cameras;
using namespace Models;
using namespace Lights;
using namespace Physics;
using namespace Forces;
using namespace std;
using namespace glm;
using namespace chrono;
using namespace nlohmann;

// TODO: should JSON strings be consts or defined?

Scene_Manager Scene_Manager::instance;

Scene_Manager& Scene_Manager::Get() {
	return instance;
}

Scene_Manager::Scene_Manager() {}

Scene_Manager::~Scene_Manager() {
	delete headNode;
	for (Camera* cam : cameras) delete cam;
	for (Light* light : lights) delete light;
}

// Any setups before the actual drawing begins are placed here
void Scene_Manager::NotifyBeginFrame() {
	//model_manager->Update();
	HandleInputs();

	auto timeNow = high_resolution_clock::now();
	//reportFramerate(timeNow);

	if (timeNow > nextPhysicsFrame) {
		if (!pausePhysics || stepPhysics) {
			Physics_Manager::Get().Update();
			Physics_Manager::Get().Transmute();
			stepPhysics = false;
		}
		nextPhysicsFrame = timeNow + physicsFramePeriod;
	}

	prevTime = timeNow;

	headNode->UpdateMatrices();
	Model_Manager::Get().Update();
	renderer->SetCamera(activeCam);
	renderer->SetLights(lights);
}

// Anything with GL calls that draw to a framebuffer goes here
void Scene_Manager::NotifyDisplayFrame() {
	if (!pauseFrame || stepFrame) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Model_Manager::Get().Draw(renderer);
		UI_Manager::Get().Draw(renderer);

		//float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
		//float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

		glutSwapBuffers();

		stepFrame = false;
	}
}

void Scene_Manager::NotifyEndFrame() {
	UI_Manager::Get().EndFrame();
}

void Scene_Manager::NotifyReshape(int width, int height, int previous_width, int previous_height) {
	// and nothing here either D:
	glViewport(0, 0, width, height);
	activeCam->aspect = (float)width / (float)height;
	UI_Manager::Get().Resize(width, height);
}

void Scene_Manager::HandleInputs()
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
	if (Input_Manager::togglePhysicsPause) {
		pausePhysics = !pausePhysics;
	}
	if (Input_Manager::stepPhysics) {
		stepPhysics = true;
	}
	if (Input_Manager::toggleFramePause) {
		pauseFrame = !pauseFrame;
	}
	if (Input_Manager::stepFrame) {
		stepFrame = true;
	}
	Strafe *= dStrafe;
	activeCam->Strafe(Strafe.x, Strafe.y);
	if (Input_Manager::mouseLeftButton == 0) {
		Strafe.x = -Input_Manager::mouseDelta.x * dTheta;
		Strafe.y = -Input_Manager::mouseDelta.y * dTheta;
		activeCam->Orbit(Strafe.x, Strafe.y, dStrafe);
	}
	activeCam->InvalidateMatrices();
	Input_Manager::Update();
}

void Scene_Manager::Init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	sceneInitialized = false;

	Shader_Manager::Get().Init();
	Texture_Manager::Get().Init();
	Material_Manager::Get().Init();
	Model_Manager::Get().Init();
	UI_Manager::Get().Init();
	Physics_Manager::Get().Init();
	ForwardRenderer::Get().Init();

	renderer = &ForwardRenderer::Get();
}

void Scene_Manager::SetRenderer(IRenderer* renderer) {
	this->renderer = renderer;
}

// TODO::add a "json_utils.h"
void Scene_Manager::BuildSceneFromJSON(string &filename) {
	string jsonString = ReadFile(filename);
	json j = json::parse(jsonString);

	//Create shaders
	Shader_Manager::Get().CreateProgram("single_color", j["Shaders"]["SingleColor"]["Vertex"], j["Shaders"]["SingleColor"]["Fragment"]);
	Shader_Manager::Get().CreateProgram("lambertian", j["Shaders"]["Lambertian"]["Vertex"], j["Shaders"]["Lambertian"]["Fragment"]);
	
	//Load textures, materials, models, and physics
	Texture_Manager::Get().LoadFromJSON(j);
	Material_Manager::Get().LoadFromJSON(j);
	Model_Manager::Get().LoadFromJSON(j);
	Physics_Manager::Get().LoadFromJSON(j);

	//Container to temporarily hold all the GameObjects, until we place them in the scene.
	unordered_map<string, GameObject*> gameObjects;
	for (auto kv : Model_Manager::Get().GetModelList()) {
		gameObjects[kv.first] = kv.second;
	}
	for (auto kv : Model_Manager::Get().GetCloneList()) {
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
			vec3 intensity(l["Intensity"][0], l["Intensity"][1], l["Intensity"][2]);	
			Light* light = new Light(it.key(), color, intensity, l["Cutoff"]);
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
	Texture_Manager::Get().WriteToJSON(j);
	Material_Manager::Get().WriteToJSON(j);
	Model_Manager::Get().WriteToJSON(j);
	Model_Manager::Get().WriteModelsToSourceJSON();
	Physics_Manager::Get().WriteToJSON(j);
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
	//Initialize rendering settings
	Init();

	//Create shaders
	Shader_Manager::Get().CreateProgram("single_color", "Shaders\\standard.vert", "Shaders\\single_color.frag");
	Shader_Manager::Get().CreateProgram("lambertian", "Shaders\\standard.vert", "Shaders\\lambertian.frag");
	Shader_Manager::Get().CreateProgram("multi_lambertian", "Shaders\\offset.vert", "Shaders\\lambertian.frag");
	Shader_Manager::Get().CreateProgram("screen_quad", "Shaders\\screen_quad.vert", "Shaders\\screen_quad.frag");

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
	lights[1]->AddToSceneTree(headNode, vec3(10), norot, one);
	
	// Make a square. It's called triangle.
	vector<VertexFormat> vertices = { VertexFormat(vec4(0, 1, 0, 1), vec3(0, -1, 0), vec2(0, 0), vec4(1, 0, 0, 1)),
		VertexFormat(vec4(1, 1, 0, 1), vec3(0, -1, 0), vec2(1, 0), vec4(1, 0, 0, 1)),
		VertexFormat(vec4(1, 0, 1, 1), vec3(0, -1, 0), vec2(1, 1), vec4(1, 0, 0, 1)),
		VertexFormat(vec4(0, 1, 1, 1), vec3(0, -1, 0), vec2(0, 1), vec4(1, 0, 0, 1)), };
	vector<unsigned int> indices{ 0, 1, 2, 0, 2, 3 };
	Model* triangle = Model_Manager::Get().CreateAndAdd("Triangle", vertices, indices);
	triangle->source = "ModelSources\\Triangle.json";
	triangle->AddToSceneTree(headNode, vec3(1.0f), -aLittleRot, one * 3.0f, false);
	Texture* tex = Texture_Manager::Get().LoadandAddTexture("Images\\test.bmp");
	dynamic_cast<Materials::LambertianMaterial*>(triangle->GetFragmentMat("Main"))->diffuseTexture = tex;

	//// Get a sphere copy
	//Model* sphere = Model_Manager::Get().PromoteToModel(Model_Manager::Get().GetSphereCopy("Sphere"));
	//sphere->AddToSceneTree(headNode, vec3(2.0f), aLittleRot, one, true);
	//sphere->SetFragmentMat("Main", triangle->GetFragmentMat("Main"));
	//Model* sphere2 = model_manager->GetSphereCopy("SphereTwo");
	//sphere2->addToSceneTree(headNode, sphere->name, vec3(0.0f, 3.0f, 1.0f), norot, one);

	//// Get a cube copy
	//Model* cube = Model_Manager::Get().PromoteToModel(Model_Manager::Get().GetCubeCopy("Cube"));
	//cube->AddToSceneTree(headNode, vec3(3, 2, 3), aLittelMoreRot, one, true);
	//Texture* cubeTex = Texture_Manager::Get().LoadandAddTexture("Images\\cubeTest.bmp");
	//cube->SetFragmentMat("Main", new Materials::LambertianMaterial(vec4(1.0f), cubeTex));

	// Create the main camera
	activeCam = new PerspectiveCamera("Main Camera");
	activeCam->AddToSceneTree(headNode, zero, norot, one);
	activeCam->LookAt(vec3(-1, -1, 2), vec3(0.5, 0.5, 0.5), vec3(0, 0, 1));
	cameras.push_back(activeCam);

	// Spam some sphere clones
	//for (int i = 0; i < 10; i++) {
	//	for (int j = 0; j < 10; j++) {
	//		string name = "Sphere" + to_string(10 * i + j);
	//		ModelClone* sphere = Model_Manager::Get()->GetSphereCopy(name);
	//		sphere->AddToSceneTree(headNode, vec3(i, j, 0), norot, one * 0.5f);
	//	}
	//}

	sceneInitialized = true;
	pauseFrame = false;
	stepFrame = false;
	Input_Manager::RegisterCallbacks();
	headNode->UpdateMatrices();
	//SaveSceneToJSON("JSON\\testScedne.json");

	//Build test cloth
	//Model* clothModel = Model_Manager::Get().PromoteToModel(Model_Manager::Get().GetRectCopy("ClothModel"));
	//clothModel->AddToSceneTree(headNode, vec3(0, 0, 1), norot, vec3(5, 5, 1), true);
	//clothModel->SetFragmentMat("Main", triangle->GetFragmentMat("Main"));

	//headNode->UpdateMatrices();

	////Continue building test cloth
	//Cloth* cloth = new Cloth(clothModel, 0, 200);
	//cloth->AddForce(new ConstantForce(vec3(0, 0, -9.81f)));
	//Physics_Manager::Get().AddPhysicsObject(cloth);
	
	//Create physics forces
	Force* gravity = new ConstantForce(vec3(0, 0, -9.81f));
	Force* x_min_bound = new BoundingForce(vec3(1, 0, 0));
	Force* y_min_bound = new BoundingForce(vec3(0, 1, 0));
	Force* z_min_bound = new BoundingForce(vec3(0, 0, 1));
	Force* x_max_bound = new BoundingForce(vec3(-1, 0, 0));
	Force* y_max_bound = new BoundingForce(vec3(0, -1, 0));
	Force* z_max_bound = new BoundingForce(vec3(0, 0, -1));
	Physics_Manager::Get().ForceList["Gravity"] = gravity;
	Physics_Manager::Get().ForceList["x_min_bound"] = x_min_bound;
	Physics_Manager::Get().ForceList["y_min_bound"] = y_min_bound;
	Physics_Manager::Get().ForceList["z_min_bound"] = z_min_bound;
	Physics_Manager::Get().ForceList["x_max_bound"] = x_max_bound;
	Physics_Manager::Get().ForceList["y_max_bound"] = y_max_bound;
	Physics_Manager::Get().ForceList["z_max_bound"] = z_max_bound;


	//Build test fluid
	Model* fluidModel = Model_Manager::Get().PromoteToModel(Model_Manager::Get().GetSphereCopy("FluidModel"));
	fluidModel->AddToSceneTree(headNode, vec3(0, 0, 0), norot, vec3(0.025, 0.025, 0.025), true);
	fluidModel->SetFragmentMat("Main", new Materials::SphereFluidMaterial());

	vector<vec3> positions;
	vec3 offset(0.1, 0.1, 0.1);
	for (float z = 0; z <= 0.6f; z += 0.06f) {
		for (float y = 0; y <= 0.18f; y += 0.06f) {
			for (float x = 0; x <= 0.6f; x += 0.06f) {
				positions.push_back(vec3(x, y, z) + offset);
			}
		}
	}

	Fluid* fluid = new Fluid(fluidModel, positions);
	
	for (auto kv : Physics_Manager::Get().ForceList) {
		fluid->AddForce(kv.first, kv.second);
	}

	Physics_Manager::Get().AddPhysicsObject(fluid);


	//Physics manager setup code
	Physics_Manager::Get().dt = 1.0f / 30.0f;
	physicsFramePeriod = high_resolution_clock::duration(33333333);
	pausePhysics = true;
	stepPhysics = false;
	Physics_Manager::Get().InitializeParticles();
	//TODO: InitializeParticles should respect fixed particles
	//cloth->FixParticle(0);
	//cloth->FixParticle(10);
	//cloth->FixParticle(110);
	//cloth->FixParticle(120);
	Physics_Manager::Get().Transmute();

	// Add stuff to UI_Manager
	UI_Manager::Get().RegisterDebuggable(fluid);
}

float Scene_Manager::calculateFramerate(high_resolution_clock::time_point &timeNow) {
	modularFrame++;
	modularFrame %= 100;
	frameTimes[modularFrame] = duration_cast<microseconds>(timeNow - prevTime).count() / 1000000.0f;
	float totalTime = 0;
	for (int i = 0; i < 100; i++) {
		totalTime += frameTimes[i];
	}

	return round(1000.0f / totalTime) / 10.0f;
}

void Scene_Manager::buildSceneTreeBranch(GameObject* node, GameObject* parent, json branch, unordered_map<string, GameObject*> &gameObjects) {
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

//json Scene_Manager::writeBranchToJSON(GameObject* node) {
//	json j;
//	for (GameObject* childNode : node->children) {
//		j[childNode->name] = writeBranchToJSON(childNode);
//	}
//	return j;
//}