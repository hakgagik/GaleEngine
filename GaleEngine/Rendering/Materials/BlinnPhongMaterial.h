#pragma once
#include "IMaterial.h"
#include "../Texture.h"
#include <glm/glm.hpp>

namespace Rendering {
	namespace Materials {
		struct BlinnPhongMaterial : public IMaterial {
		public:
			virtual ~BlinnPhongMaterial() override {}

			glm::vec4 diffuseColor = glm::vec4(1.0);
			glm::vec4 specularColor = glm::vec4(1.0);
			glm::float32 exponent = 50.0f;

			// Optional textures
			Texture* diffuseTexture = nullptr;
			Texture* specularTexture = nullptr;
			Texture* exponentTexture = nullptr;

			BlinnPhongMaterial(glm::vec4 diffColor = glm::vec4(1.0f), glm::vec4 specColor = glm::vec4(1.0f), float exp = 50) {
				diffuseColor = diffColor;
				specularColor = specColor;
				exponent = exp;
			}
			BlinnPhongMaterial(Texture* diffTexture, Texture* specTexture = nullptr, Texture* expTexture = nullptr, glm::vec4 diffColor = glm::vec4(1.0f), glm::vec4 specColor = glm::vec4(1.0f), float exp = 50.0f) {
				diffuseColor = diffColor;
				specularColor = specColor;
				exponent = exp;
				diffuseTexture = diffTexture;
				specularTexture = specTexture;
				exponentTexture = expTexture;
			}

		};
	}
}