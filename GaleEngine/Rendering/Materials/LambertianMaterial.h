#pragma once
#include "IMaterial.h"
#include "../Texture.h"
#include <glm/glm.hpp>

namespace Rendering {
	namespace Materials {
		struct LambertianMaterial : public IMaterial {
		public:
			virtual ~LambertianMaterial() override {}

			glm::vec4 diffuseColor = glm::vec4(1.0f);
			Texture* diffuseTexture = nullptr;

			LambertianMaterial(glm::vec4 color = glm::vec4(1.0f), Texture* texture = nullptr) {
				diffuseColor = color;
				diffuseTexture = texture;
			}
		};
	}
}