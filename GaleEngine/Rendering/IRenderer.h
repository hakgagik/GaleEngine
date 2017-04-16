#pragma once
#include <vector>
#include <string>

namespace Rendering {
	namespace GameObjects {
		namespace Cameras {
			class Camera;
		}
		namespace Lights {
			class Light;
		}
		namespace Models {
			class Model;
			class ModelClone;
		}
	}
	class IRenderer {
	public:
		virtual void Init() = 0;
		virtual void Render(GameObjects::Models::Model* model) = 0;
		virtual void Render(GameObjects::Models::ModelClone* model) = 0;
		virtual void SetLights(std::vector<GameObjects::Lights::Light*> lights) = 0;
		virtual void SetCamera(GameObjects::Cameras::Camera* camera) = 0;
		//virtual void RenderText(std::string& text, float x, float y, float sx, float sy) = 0;

	protected:
		std::vector<GameObjects::Lights::Light*> lights;
		GameObjects::Cameras::Camera* camera;
	};
}