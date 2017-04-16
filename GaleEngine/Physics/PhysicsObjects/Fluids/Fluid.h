#pragma once
#include "../PhysicsObject.h"
#include <vector>
#include <glm/glm.hpp>

namespace Rendering {
	namespace GameObjects {
		namespace Models {
			class Model;
		}
	}
}

namespace Physics {
	namespace Constraints {
		class DensityConstraint;
	}

	namespace PhysicsObjects {
		namespace Fluids {
			class Fluid : public PhysicsObject {
			public:
				Fluid();
				Fluid(Rendering::GameObjects::Models::Model* model, std::vector<glm::vec3> &positions, float particleMass = 1, float restDensity = -1, float densityStiffness = 0.999f);
				virtual ~Fluid();
				virtual void GenerateConstraints(float dt) override;
				virtual void Project(int iterations) override;
				virtual void FinalizeParticles(float dt) override;
				virtual void DeleteParticle(Particles::Particle* particle) override;
				virtual void Transmute() override;
				virtual std::vector<std::string> GetDebugOutput() override;
				virtual nlohmann::json GetJSON() override;
			protected:
				std::vector<Constraints::DensityConstraint*> densityConstraintList;
				float restDensity;
				float densityStiffness;
				bool particlesChanged;
			};
		}
	}
}