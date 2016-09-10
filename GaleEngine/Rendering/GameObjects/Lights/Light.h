#pragma once
#include "../IGameObject.h"
#include <glm/glm.hpp>

namespace Rendering {
	namespace GameObjects {
		namespace Lights {
			class Light : public IGameObject {
			public:
				glm::float32 cutoff;
				glm::vec3 color;
				glm::vec3 attenuation;
				// Later: cookie, shadow proxies, etc
				Light(std::string name, glm::vec3 color = glm::vec3(1, 1, 1), glm::vec3 attenuation = glm::vec3(0, 0, 0.1), glm::float32 cutoff = 1000000.0f);
				~Light();
				virtual void Destroy() override;
				virtual nlohmann::json GetSourceJSON() const override;
			};
		}
	}
}