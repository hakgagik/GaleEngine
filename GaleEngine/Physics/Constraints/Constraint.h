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

			virtual void Update() = 0;
			virtual bool ContainsParticle(Particles::Particle* particle) = 0;
			
			std::unordered_map<Particles::Particle*, glm::vec3> particleGradients;
			float stiffness;
			float lambda;
			float currentValue;
		};
		inline Constraint::~Constraint() {}
	}
}