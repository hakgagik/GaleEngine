// Modified from http://in2gpu.com/opengl-3/
#pragma once
#include "../Core/ISceneListener.h"
#include "../lib/json.hpp"
#include <vector>
#include <unordered_map>
#include <chrono>


namespace Rendering {
	class IRenderer;
	namespace GameObjects {
		class IGameObject;
		namespace Cameras {
			class Camera;
		}
		namespace Lights {
			class Light;
		}
	}
}

namespace Managers {
	class Shader_Manager;
	class Model_Manager;
	class Texture_Manager;
	class Material_Manager;
	class Physics_Manager;

	class Scene_Manager : public Core::ISceneListener {
	public:
		static Scene_Manager& Get();

		~Scene_Manager();

		virtual void NotifyBeginFrame();
		virtual void NotifyDisplayFrame();
		virtual void NotifyEndFrame();
		virtual void NotifyReshape(int width, int height, int previous_width, int previous_height);
		virtual void HandleInputs();

		void Init();
		void SetRenderer(Rendering::IRenderer* renderer);
		void BuildSceneFromJSON(std::string &filename);
		void SaveSceneToJSON(const std::string &filename);
		void SetupTestScene();
		std::string ReadFile(const std::string &filename);

	private:
		static Scene_Manager instance;

		std::chrono::high_resolution_clock::time_point prevTime;
		std::chrono::high_resolution_clock::time_point nextPhysicsFrame;
		std::chrono::high_resolution_clock::duration physicsFramePeriod;
		float frameTimes[100];
		int modularFrame;
		float physicsDt;

		Scene_Manager();
		Scene_Manager(const Scene_Manager &);
		Rendering::IRenderer* renderer;

		Rendering::GameObjects::Cameras::Camera* activeCam;
		std::vector<Rendering::GameObjects::Cameras::Camera*> cameras; // May need to switch this to a camera manager at some point
		std::vector<Rendering::GameObjects::Lights::Light*> lights;
		Rendering::GameObjects::IGameObject* headNode;

		float dStrafe = 0.1f;
		float dTheta = 0.01308996939f;
		bool pausePhysics;
		bool stepPhysics;

		void reportFramerate(std::chrono::high_resolution_clock::time_point &timeNow);

		//nlohmann::json writeBranchToJSON(Rendering::GameObjects::IGameObject* node);
		void buildSceneTreeBranch(Rendering::GameObjects::IGameObject* node, Rendering::GameObjects::IGameObject* parent, nlohmann::json branch, std::unordered_map<std::string, Rendering::GameObjects::IGameObject*> &gameObjects);
	};
}