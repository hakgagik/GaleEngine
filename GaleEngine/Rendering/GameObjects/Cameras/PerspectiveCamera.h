#pragma once
#include "Camera.h"
#include <glm/glm.hpp>

namespace Rendering{
	namespace GameObjects {
		namespace Cameras {
			class PerspectiveCamera : public Camera {
			public:
				static const glm::vec3 DEFAULT_EYE;
				static const glm::vec3 DEFAULT_TARGET;
				static const glm::vec3 DEFAULT_UP;
				static const glm::vec3 VERTICAL;

				float fovy;

				PerspectiveCamera(float near = 0.1f, float far = 100.0f, float aspect = 1.0f, float fovy = 1.0471975512f);
				virtual ~PerspectiveCamera() override;
				virtual void Destroy() override;
				virtual glm::mat4 GetProjMatrix() override;
				virtual glm::mat4 GetViewMatrix() override;
				virtual void LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up) override;
				virtual void Orbit(float dTheta, float dPhi, float boundSize = 0.001f) override;
				virtual void Dolly(float dRho) override;
				virtual void Zoom(float zoomFactor) override;
				virtual void Strafe(float dx, float dy) override;

				virtual nlohmann::json GetJSON() override;
			protected:
				//void updateFrame();
				void updateLocalMatrices() override;
				//bool frameUpdated;
			};
		}
	}
}