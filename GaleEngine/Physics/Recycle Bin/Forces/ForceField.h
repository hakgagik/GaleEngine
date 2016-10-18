#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <unordered_set>
#include "../../lib/json.hpp"

namespace Physics {
	namespace Particles {
		class Particle;
	}
	namespace Forces {
		class ForceField {
		public:
			virtual ~ForceField() = 0;

			virtual glm::vec3 GetForce(glm::vec3& position, glm::vec3& velocity) = 0;
			virtual void Apply() = 0;
			virtual void NotifyParticleDeleted(Particles::Particle* particle) = 0;
			virtual nlohmann::json GetJSON() = 0;
		protected:
			std::unordered_set<Particles::Particle*> affectedParticles;
		};
	inline ForceField::~ForceField() {}
	}
}