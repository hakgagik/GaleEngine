#pragma once

namespace Physics {
	namespace Particles {
		class Particle;
	}
	namespace Constraints {
		class Constraint;
		class ConstraintGroup {
		public:
			virtual void Project(int iterations) = 0;
			virtual void DeleteParticle(Particles::Particle* particle) = 0;
		};
	}
}