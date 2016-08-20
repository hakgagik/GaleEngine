// Modified from http://in2gpu.com/opengl-3/
#pragma once
#include "../Core/ISceneListener.h"
#include "../lib/json.hpp"
#include <vector>


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
		Scene_Manager();
		~Scene_Manager();

		virtual void notifyBeginFrame();
		virtual void notifyDisplayFrame();
		virtual void notifyEndFrame();
		virtual void notifyReshape(int width, int height, int previous_width, int previous_height);
		virtual void handleInputs();

		void SetRenderer(Rendering::IRenderer* renderer);
		void BuildSceneFromJSON(std::string &filename);
		void SaveSceneToJSON(const std::string &filename);
		void SetupTestScene();
		std::string ReadFile(std::string &filename);

	private:
		Shader_Manager* shader_manager;
		Model_Manager* model_manager;
		Texture_Manager* texture_manager;
		Material_Manager* material_manager;
		Physics_Manager* physics_manager;

		Rendering::IRenderer* renderer;

		Rendering::GameObjects::Cameras::Camera* activeCam;
		std::vector<Rendering::GameObjects::Cameras::Camera*> cameras; // May need to switch this to a camera manager at some point
		std::vector<Rendering::GameObjects::Lights::Light*> lights;
		Rendering::GameObjects::IGameObject* headNode;

		float dStrafe = 0.1f;
		float dTheta = 0.01308996939f;

		nlohmann::json writeBranchToJSON(Rendering::GameObjects::IGameObject* node);
	};
}