#include <glm/glm.hpp>
#include <vector>

#pragma once
namespace Physics {
	namespace Constraints {
		class Constraint;
	}
	namespace Particles {
		class Particle {
		public:
			Particle();
			Particle(glm::vec3 pos, glm::vec3 vel = glm::vec3(0), float mass = 1.0f);
			virtual ~Particle();

			glm::vec3 x, x0, p;
			glm::vec3 v, v0;
			float m, w;
			bool isFixed;
			//std::vector<Forces::ForceField*> affectingForces;

			void Fix();
			void Unfix();
		};
	}
}