#pragma once
#include "Force.h"
#include <unordered_map>

namespace Physics {
	namespace Particles {
		class Particle;
	}
	namespace Forces {
		class BoundingForce : public Force {
		public:
			BoundingForce(glm::vec3 &direction);
			virtual glm::vec3 GetForce(Particles::Particle* particle) override;
			virtual void AddParticle(Particles::Particle* particle);
		protected:
			glm::vec3 dir;
			std::unordered_map<Particles::Particle*, glm::vec3> affectedParticles;
		};
	}
}