#pragma once
#include <glm/glm.hpp>
#include <unordered_map>


namespace Physics {
	namespace Particles {
		class Particle;
	}

	namespace Constraints {
		class Constraint {
		public:
			virtual ~Constraint();

			virtual void UpdateDerivs() = 0;
			virtual bool ContainsParticle(Particles::Particle* particle) = 0;

			std::unordered_map<Particles::Particle*, glm::vec3> ParticleGradients;
			float stiffness;
			//float currentValue;
			float s;
		};
		inline Constraint::~Constraint() {}
	}
}