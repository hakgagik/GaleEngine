// Modified from http://in2gpu.com/opengl-3/
#pragma once
#include "../Core/ISceneListener.h"
#include "../Core/IInputListener.h"
#include "../lib/json.hpp"
#include <vector>
#include <unordered_map>
#include <chrono>


namespace Rendering {
	class IRenderer;
	namespace GameObjects {
		class GameObject;
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

	class Scene_Manager : public Core::ISceneListener, public Core::IInputListener {
	public:
		static Scene_Manager& Get();

		~Scene_Manager();

		virtual void NotifySceneInit() override;
		virtual void NotifyBeginFrame() override;
		virtual void NotifyDisplayFrame() override;
		virtual void NotifyEndFrame() override;
		virtual void NotifyReshape(int width, int height, int previous_width, int previous_height) override;
		virtual void NotifySceneClose() override;

		virtual void NotifyKeyDown(char key) override;
		virtual void NotifyKeyUp(char key) override;
		virtual void NotifySpecialKeyDown(GLint key) override;
		virtual void NotifySpecialKeyUp(GLint key) override;
		virtual void NotifyMouseDown(int key) override;
		virtual void NotifyMouseUp(int key) override;
		virtual void NotifyMouseMove(int dx, int dy) override;
		virtual void NotifyMouseWheel(int dz) override;

		void Init();
		void SetRenderer(Rendering::IRenderer* renderer);
		void BuildSceneFromJSON(std::string &filename);
		void SaveSceneToJSON(const std::string &filename);
		std::string ReadFile(const std::string &filename);

	private:
		static Scene_Manager instance;
		
		void setupTestScene();
		
		std::chrono::high_resolution_clock::time_point nextPhysicsFrame;
		std::chrono::high_resolution_clock::duration physicsFramePeriod;
		std::vector<std::chrono::high_resolution_clock::time_point> frameTimes;
		const int framesToTrack = 5;
		int modularFrame;

		Scene_Manager();
		Scene_Manager(const Scene_Manager &);
		Rendering::IRenderer* renderer;

		Rendering::GameObjects::Cameras::Camera* activeCam;
		std::vector<Rendering::GameObjects::Cameras::Camera*> cameras; // May need to switch this to a camera manager at some point
		std::vector<Rendering::GameObjects::Lights::Light*> lights;
		Rendering::GameObjects::GameObject* headNode;

		float dStrafe = 0.1f;
		float dTheta = 0.01308996939f;
		bool pausePhysics;
		bool stepPhysics;
		bool pauseFrame;
		bool stepFrame;
		//bool fullscreen;

		float calculateFramerate(std::chrono::high_resolution_clock::time_point &timeNow);
		
		void toggleFullscreen();
		void exit();

		//nlohmann::json writeBranchToJSON(Rendering::GameObjects::GameObject* node);
		void buildSceneTreeBranch(Rendering::GameObjects::GameObject* node, Rendering::GameObjects::GameObject* parent, nlohmann::json branch, std::unordered_map<std::string, Rendering::GameObjects::GameObject*> &gameObjects);
	};
}