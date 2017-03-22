#pragma once
#include <glm/glm.hpp>

namespace Physics {
	namespace Collisions {
		struct Boundary {
		public:
			glm::vec3 P;
			glm::vec3 N;
			Boundary(glm::vec3 P, glm::vec3 N) {
				this->P = P;
				this->N = glm::normalize(N);
			}
		};
	}
}