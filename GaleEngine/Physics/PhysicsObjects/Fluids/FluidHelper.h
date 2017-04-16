#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace Physics {
	namespace Particles {
		class Particle;
	}
	namespace Constraints {
		class DensityConstraint;
	}
	namespace PhysicsObjects {
		namespace Fluids {
			class FluidHelper {
			public:
				static FluidHelper &Get();
				static FluidHelper GetThrowaway();
				
				static void Init();

				~FluidHelper();

				void AddParticle(Constraints::DensityConstraint* constraint);
				void AddParticles(std::vector<Constraints::DensityConstraint*> &constraints); // Add new fluid particles
				void DeleteParticle(Physics::Particles::Particle* particle);
				void Update(); // Re-bin all particles
				void ReBin(float newH); // Re-bin all particles with a new bin size
				void Clear(); // Clear all bins and particles

				std::unordered_map<Physics::Particles::Particle*, Constraints::DensityConstraint*> FluidParticles; // A list of all particles that exist in the bins
				std::unordered_map<glm::ivec3, std::vector<Particles::Particle*>> Bins; // All of the particles sorted into bins. #binning
			private:
				static FluidHelper instance;
				FluidHelper();
				float h = 0.2f;
			};
		}
	}
}