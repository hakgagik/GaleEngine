#pragma once
#include "IMaterial.h"
#include <glm/glm.hpp>

namespace Rendering {
	namespace Materials {
		struct SingleColorMaterial : public IMaterial {
		public:
			virtual ~SingleColorMaterial() override {}
			glm::vec4 diffuseColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
			SingleColorMaterial(glm::vec4 color = glm::vec4(1)) {
				diffuseColor = color;
			}
		};
	}
}