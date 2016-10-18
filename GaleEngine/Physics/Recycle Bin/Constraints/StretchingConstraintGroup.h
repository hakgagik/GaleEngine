#pragma once
#include "ConstraintGroup.h"
#include <vector>

namespace Physics {
	namespace Constraints {
		class StretchingConstraint;
		class StretchingConstraintGroup : ConstraintGroup {

			virtual void Project(int iterations) override;
			virtual void DeleteParticle(Particles::Particle* particle) override;
			void CreateStretchingConstraint(Particles::Particle* particle1, Particles::Particle* particle2);
			void CreateStretchingConstraint(Particles::Particle* particle1, Particles::Particle* particle2, float distance);
		private:
			std::vector<StretchingConstraint*> constraints;
		};
	}
}