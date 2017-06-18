#pragma once
#include "Constraint.h"

namespace Physics {
	namespace Constraints {
		class StretchingConstraint : public Constraint {
		public:
			StretchingConstraint(Particles::Particle* particle1, Particles::Particle* particle2, float stiffness);
			StretchingConstraint(Particles::Particle* particle1, Particles::Particle* particle2, float restDistance, float stiffnes);

			virtual void UpdateDerivs() override;
			bool ContainsParticle(Particles::Particle* particle) override;
			bool ContainsParticles(Particles::Particle* p1, Particles::Particle* p2);

			float restDistance;
			//Particles::Particle* particle1;
			//Particles::Particle* particle2;
		};
	}
}