#pragma once
#include <glm/glm.hpp>
#include <vector>


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

			std::vector<Particles::Particle*> ParticleList;
			std::vector<glm::vec3> ParticleGradients;
			float stiffness;
			//float currentValue;
			float s;
		};
		inline Constraint::~Constraint() {}
	}
}