#pragma once
#include "IRenderer.h"
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <string>

namespace Rendering {

	class Texture;
	namespace GameObjects {
		class IGameObject;
		namespace Models {
			class Model;
			class ModelClone;
			class Fragment;
		}
		namespace Cameras {
			class Camera;
		}
		namespace Lights {
			class Light;
		}
	}
	namespace Materials {
		struct SingleColorMaterial;
		struct LambertianMaterial;
		struct BlinnPhongMaterial;
	}

	class ForwardRenderer : public IRenderer {
	public:
		ForwardRenderer();
		virtual ~ForwardRenderer() override;

		virtual void Render(GameObjects::Models::Model* model) override;
		virtual void Render(GameObjects::Models::ModelClone* model) override;
		virtual void setCamera(GameObjects::Cameras::Camera* camera) override;
		virtual void setLights(std::vector<GameObjects::Lights::Light*> light) override;

	private:
		const static GLenum textureIndexMap[];
		const static int MAX_LIGHTS;

		glm::mat4 MVPMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat3 normalMatrix;
		glm::mat4 modelViewMatrix;
		glm::mat4 inverseViewMatrix;

		std::vector<glm::vec3> lightPositions;
		std::vector<glm::vec3> lightAttenuations;
		std::vector<glm::vec3> lightColors;
		std::vector<glm::float32> lightCutoffs;

		//void bindPosNorTexTanAttributes(GLuint program, Models::Model* model, Models::Fragment* fragment);
		void calculateOrientationMatrices(GameObjects::IGameObject* gameObject);
		void setMatrixUniforms(GLuint program);
		void setLightUniforms(GLuint program);
		void useTexture(GLuint texIndex, const GLchar* uniformName, const GLchar* hasUniformName, Texture* texture, int textureUnit);
		void unuseTexture(GLuint program, Texture* texture);
		void renderFromModel(GameObjects::Models::Model* model);
		void renderFragment(GameObjects::Models::Model* model, GameObjects::Models::Fragment* fragment, Rendering::Materials::SingleColorMaterial* mat);
		void renderFragment(GameObjects::Models::Model* model, GameObjects::Models::Fragment* fragment, Rendering::Materials::LambertianMaterial* mat);
		void renderFragment(GameObjects::Models::Model* model, GameObjects::Models::Fragment* fragment, Rendering::Materials::BlinnPhongMaterial* mat);
	};
}