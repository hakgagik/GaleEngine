#pragma once
#include "IMaterial.h"
#include "../Texture.h"
#include "glm/glm.hpp"

namespace Rendering {
	namespace Materials {
		struct ScreenQuadMaterial : public IMaterial {
		public:
			virtual ~ScreenQuadMaterial() override {}
			glm::vec4 posAndScale = glm::vec4(0, 0, 1, 1);
			glm::vec4 tint = glm::vec4(1);
			const Texture* texture = nullptr;

			ScreenQuadMaterial(glm::vec4 posAndScale = glm::vec4(0, 0, 1, 1), const Texture* texture = nullptr, glm::vec4 tint = glm::vec4(1)) {
				this->posAndScale = posAndScale;
				this->texture = texture;
				this->tint = tint;
			}
		};
	}
}