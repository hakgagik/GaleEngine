#pragma once
#include "IMaterial.h"
#include <glm/glm.hpp>
#include <vector>

namespace Rendering {
	namespace Materials {
		struct SphereFluidMaterial : public IMaterial {
		public:
			virtual ~SphereFluidMaterial() override {}
			glm::vec4 diffuseColor = glm::vec4(0.0, 0.2, 1.0, 1.0);
			std::vector<glm::vec3> positions;
			SphereFluidMaterial(glm::vec4 color = glm::vec4(0.0, 0.2, 1.0, 1.0)) {
				diffuseColor = color;
			}
		};
	}
}