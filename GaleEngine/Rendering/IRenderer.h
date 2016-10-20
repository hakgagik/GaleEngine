#pragma once
#include <vector>

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

		virtual ~IRenderer() = 0;

		virtual void Init() = 0;
		virtual void Render(GameObjects::Models::Model* model) = 0;
		virtual void Render(GameObjects::Models::ModelClone* model) = 0;
		virtual void setLights(std::vector<GameObjects::Lights::Light*> lights) = 0;
		virtual void setCamera(GameObjects::Cameras::Camera* camera) = 0;

	protected:
		std::vector<GameObjects::Lights::Light*> lights;
		GameObjects::Cameras::Camera* camera;
	};

	inline IRenderer::~IRenderer() {}
}