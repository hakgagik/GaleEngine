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
		class Constraint;
		class StretchingConstraint;
		class FaceBendingConstraint;
	}
	namespace PhysicsObjects {
		namespace Solids {
			class Cloth : public PhysicsObject {
			public:
				Cloth();
				Cloth(Rendering::GameObjects::Models::Model* model, unsigned int indexStart, unsigned int numTriangles, float stretchingStiffness = 1.0f, float bendingStiffness = 1.0f);
				virtual ~Cloth();
				virtual void Project(int iterations) override;
				virtual void DeleteParticle(Particles::Particle* particle) override;
				virtual void Transmute();
				void FixParticle(unsigned int particleIndex);
				virtual nlohmann::json GetJSON();

			protected:
				unsigned int indexStart;
				unsigned int numTriangles;
				Rendering::GameObjects::Models::Model* model;

				std::vector<Constraints::StretchingConstraint*> stretchingConstraintList;
				std::vector<Constraints::FaceBendingConstraint*> faceBendingConstraintList;
			};
		}
	}
}