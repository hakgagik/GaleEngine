#pragma once
#include "Constraint.h"
#include <glm/glm.hpp>

namespace Physics {
	namespace Particles {
		class Particle;
	}

	namespace Constraints {
		class DensityConstraint : public Constraint {
		public:
			static float SetH(float newH);
			static float Poly6Kernel(float r);
			static float SpikeyKernel(float r);
			static float DelSpikeyKernel(float r);

			DensityConstraint(Particles::Particle* center, float density);
			
			virtual void UpdateDerivs() override;
			virtual bool ContainsParticle(Particles::Particle* particle) override;
		private:
			static glm::vec3 h;
			static float poly6Factor;
			static float spikyFactor;
			static float delSpikyFactor;

			float density;
			Particles::Particle* center;
		};
	}
}