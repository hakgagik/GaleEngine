#pragma once
#include "IRenderer.h"
#include "GL/glew.h"
#include <glm/glm.hpp>

namespace Rendering {

	class Texture;
	namespace GameObjects {
		class GameObject;
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
		struct SphereFluidMaterial;
		struct ScreenQuadMaterial;
	}

	class ForwardRenderer : public IRenderer {
	public:
		static ForwardRenderer& Get();

		virtual ~ForwardRenderer();
		
		virtual void Init() override;
		virtual void Render(GameObjects::Models::Model* model) override;
		virtual void Render(GameObjects::Models::ModelClone* model) override;
		virtual void SetCamera(GameObjects::Cameras::Camera* camera) override;
		virtual void SetLights(std::vector<GameObjects::Lights::Light*> light) override;
		//virtual void RenderText(std::string& text, float x, float y, float sx, float sy) override;
	private:
		static ForwardRenderer instance;

		ForwardRenderer();

		const static int MAX_LIGHTS;

		glm::mat4 MVPMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 modelMatrix;
		glm::mat3 normalMatrix;
		glm::mat4 modelViewMatrix;
		glm::mat4 inverseViewMatrix;

		std::vector<glm::vec3> lightPositions;
		std::vector<glm::vec3> lightAttenuations;
		std::vector<glm::vec3> lightColors;
		std::vector<glm::float32> lightCutoffs;

		//void bindPosNorTexTanAttributes(GLuint program, Models::Model* model, Models::Fragment* fragment);
		void calculateOrientationMatrices(GameObjects::GameObject* gameObject);
		void setMatrixUniforms(GLuint program);
		void setLightUniforms(GLuint program);
		void useTexture(GLuint texIndex, const GLchar* uniformName, const GLchar* hasUniformName, const Texture* texture, GLuint textureUnit);
		void unuseTexture(GLuint program, Texture* texture);
		void renderFromModel(const GameObjects::Models::Model* model);
		void renderFragment(const GameObjects::Models::Model* model, const GameObjects::Models::Fragment* fragment, Rendering::Materials::SingleColorMaterial* mat);
		void renderFragment(const GameObjects::Models::Model* model, const GameObjects::Models::Fragment* fragment, Rendering::Materials::LambertianMaterial* mat);
		void renderFragment(const GameObjects::Models::Model* model, const GameObjects::Models::Fragment* fragment, Rendering::Materials::BlinnPhongMaterial* mat);
		void renderFragment(const GameObjects::Models::Model* model, const GameObjects::Models::Fragment* fragment, Rendering::Materials::SphereFluidMaterial* mat);
		void renderFragment(const GameObjects::Models::Model* model, const GameObjects::Models::Fragment* fragment, Rendering::Materials::ScreenQuadMaterial* mat);
	};
}