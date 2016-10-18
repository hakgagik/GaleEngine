#pragma once
#include <glm/glm.hpp>

namespace Physics {
	namespace Particles {
		class Particle;
	}
	namespace Forces {
		class Force {
		public:
			virtual glm::vec3 GetForce(Particles::Particle* particle) = 0;
		};
	}
}