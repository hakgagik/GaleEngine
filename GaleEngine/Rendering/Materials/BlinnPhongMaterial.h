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
			const Texture* diffuseTexture = nullptr;
			const Texture* specularTexture = nullptr;
			const Texture* exponentTexture = nullptr;

			BlinnPhongMaterial(glm::vec4 diffColor = glm::vec4(1.0f), glm::vec4 specColor = glm::vec4(1.0f), float exp = 50) {
				diffuseColor = diffColor;
				specularColor = specColor;
				exponent = exp;
			}

			// Second constructure in case I was to specify textures instead of colors. First one has no default to avoid two default constructors
			BlinnPhongMaterial(const Texture* diffTexture, const Texture* specTexture = nullptr, const Texture* expTexture = nullptr, glm::vec4 diffColor = glm::vec4(1.0f), glm::vec4 specColor = glm::vec4(1.0f), float exp = 50.0f) {
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