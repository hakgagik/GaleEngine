#pragma once
#include "../GameObject.h"
#include <glm/glm.hpp>

namespace Rendering {
	namespace GameObjects {
		namespace Cameras {
			class Camera : public GameObject {
			protected:
				glm::vec3 target;
				glm::vec3 up;
				glm::vec3 right;
				glm::vec3 negGaze;
				glm::vec3 globalPos;

			public:
				float nearClip;
				float farClip;
				float aspect;

				Camera(std::string name) : GameObject(name) { };
				virtual glm::mat4 GetProjMatrix() = 0;
				virtual glm::mat4 GetViewMatrix() = 0;
				virtual void Destroy() override = 0;
				virtual void LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up) = 0;
				virtual void Orbit(float dTheta, float dPhi, float boundSize = 0.001f) = 0;
				virtual void Dolly(float dRho) = 0;
				virtual void Zoom(float zoomFactor) = 0;
				virtual void Strafe(float dx, float dy) = 0;
				virtual nlohmann::json GetSourceJSON() const override = 0;
			protected:
				virtual void updateLocalMatrices() override = 0;
			};
		}
	}
}