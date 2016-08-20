#pragma once
#include "Constraint.h"

namespace Physics {
	namespace Constraints {
		class StretchingConstraint : public Constraint {
		public:
			StretchingConstraint(Particles::Particle* particle1, Particles::Particle* particle2);
			StretchingConstraint(Particles::Particle* particle1, Particles::Particle* particle2, float distance);

			virtual void Update() override;
			bool ContainsParticle(Particles::Particle* particle);

			float restDistance;
			Particles::Particle* particle1;
			Particles::Particle* particle2;
		};
	}
}