#pragma once
#include "../IConstraintGroup.h"
#include "../../lib/json.hpp"
#include <unordered_map>

namespace Rendering {
	namespace GameObjects {
		namespace Models {
			class Model;
		}
	}
}

namespace Physics {
	class AABB;
	namespace Forces {
		class Force;
	}
	namespace Particles {
		class Particle;
	}
	namespace PhysicsObjects {
		class PhysicsObject : public IConstraintGroup {
		public:
			virtual ~PhysicsObject();
			virtual void AddForce(Forces::Force* force);

			virtual void InitParticles();
			virtual void ApplyForces(float dt);
			virtual void PredictPositions(float dt);
			virtual void GenerateConstraints(float dt) override;
			virtual void Project(int iterations) override = 0;
			virtual void FinalizeParticles(float dt);
			virtual void Transmute() = 0;
			virtual void UpdateAABB();

			virtual void DeleteParticle(Particles::Particle* particle) override = 0;
			virtual nlohmann::json GetJSON() = 0;

		protected:
			std::unordered_map<int, Particles::Particle*> particleList;
			std::vector<Forces::Force*> forceList;
			Rendering::GameObjects::Models::Model* model;
			AABB* boundingBox;
		};
		inline PhysicsObject::~PhysicsObject() { }
	}
}