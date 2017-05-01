#pragma once
#include "../GameObject.h"
#include <glm/glm.hpp>

namespace Rendering {
	namespace GameObjects {
		namespace Lights {
			class Light : public GameObject {
			public:
				glm::float32 Cutoff;
				glm::vec3 Color;
				glm::vec3 Intensity;
				// Later: cookie, shadow proxies, etc
				Light(std::string name, glm::vec3 color = glm::vec3(1, 1, 1), glm::vec3 intensity = glm::vec3(1, 0, 100), glm::float32 cutoff = 1000000.0f);
				~Light();
				virtual void Destroy() override;
				virtual nlohmann::json GetSourceJSON() const override;
			};
		}
	}
}