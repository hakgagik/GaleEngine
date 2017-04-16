#pragma once
#include "../PhysicsObject.h"
#include <vector>

namespace Rendering {
	namespace GameObjects {
		namespace Models {
			class Model;
		}
	}
}

namespace Physics {
	namespace Constraints {
		class StretchingConstraint;
		class FaceBendingConstraint;
	}
	namespace PhysicsObjects {
		namespace Solids {
			class Cloth : public PhysicsObject {
			public:
				Cloth();
				Cloth(Rendering::GameObjects::Models::Model* model, unsigned int indexStart, unsigned int numTriangles, float stretchingStiffness = 0.9f, float bendingStiffness = 0.9f);
				virtual ~Cloth();
				virtual void Project(int iterations) override;
				virtual void DeleteParticle(Particles::Particle* particle) override;
				virtual void Transmute();
				virtual std::vector<std::string> GetDebugOutput() override;
				void FixParticle(unsigned int particleIndex);
				virtual nlohmann::json GetJSON() override;

			protected:
				unsigned int indexStart;
				unsigned int numTriangles;

				std::vector<Constraints::StretchingConstraint*> stretchingConstraintList;
				std::vector<Constraints::FaceBendingConstraint*> faceBendingConstraintList;
			};
		}
	}
}