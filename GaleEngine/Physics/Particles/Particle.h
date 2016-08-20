#include <glm/glm.hpp>
#include <vector>

#pragma once
namespace Physics {
	namespace Forces {
		class ForceField;
	}
	namespace Constraints {
		class Constraint;
	}
	namespace Particles {
		class Particle {
		public:
			virtual ~Particle();

			glm::vec3 x, x0, p;
			glm::vec3 v, v0;
			float m, w;
			bool isFixed;
			std::vector<Forces::ForceField*> affectingForces;
		};
		inline Particle::~Particle() {}
	}
}