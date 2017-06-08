#pragma once
#include "Constraint.h"
//#include "../PhysicsObjects/Fluids/FluidHelper.h"
#include <glm/glm.hpp>

namespace Physics {
	namespace Particles {
		class Particle;
	}

	namespace PhysicsObjects {
		namespace Fluids {
			class FluidHelper;
		}
	}

	namespace Constraints {
		class DensityConstraint : public Constraint {
		public:
			//static void SetH(float newH);
			static float Poly6Kernel(float &r);
			static float Poly6Kernel_rsq(float &r);
			static float SpikeyKernel(float &r);
			static float DelSpikeyKernel(float &r);

			DensityConstraint(Particles::Particle* center, float restDensity, float stiffness);
			
			virtual void UpdateDerivs() override;
			virtual bool ContainsParticle(Particles::Particle* particle) override;
			void FindNeighbors(Physics::PhysicsObjects::Fluids::FluidHelper &fluidHelper);
			float CalculateLocalDensity();
			void SetRestDensity(float newDensity);
			glm::vec3 GetDP();
			glm::vec3 GetDV();
			glm::vec3 GetFVC();

			float CurrentDensity;
			float RestDensity;
			float Lambda;
			float Omega;
			Particles::Particle* Center;
			glm::vec3 CenterDeriv;
		private:
			static float h;
			static float h_sq;
			static float epsilon;
			static float poly6Factor;
			static float spikyFactor;
			static float delSpikyFactor;
			static float k;
			//static unsigned int n;
			static float q;
			static float sCorrDenom;
			static float c;

			float flipOrder;
		};
	}
}