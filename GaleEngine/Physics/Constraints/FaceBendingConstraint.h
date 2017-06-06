#pragma once
#include "Constraint.h"
#include <glm/glm.hpp>

namespace Physics {
	namespace Constraints {
		class FaceBendingConstraint : public Constraint {
		public:
			FaceBendingConstraint(Particles::Particle* particle1, Particles::Particle* particle2, Particles::Particle* particle3, Particles::Particle* particle4, float stiffness);
			FaceBendingConstraint(Particles::Particle* particle1, Particles::Particle* particle2, Particles::Particle* particle3, Particles::Particle* particle4, float restAngle, float stiffness);
			
			virtual void UpdateDerivs() override;
			bool ContainsParticle(Particles::Particle* particle) override;

			float restAngle;
			//Particles::Particle* particle1;
			//Particles::Particle* particle2;
			//Particles::Particle* particle3;
			//Particles::Particle* particle4;

		protected:
			float getAngle(glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, glm::vec3 &p4);
		};
	}
}