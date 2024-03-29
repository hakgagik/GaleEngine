#pragma once
#include "../IConstraintGroup.h"
#include "../../lib/json.hpp"
#include <unordered_map>
#include <vector>

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
			virtual void AddForce(std::string name, Forces::Force* force);

			virtual void InitParticles();
			virtual void ApplyForces();
			virtual void PredictPositions();
			virtual void CalculatePotentialInteractions() override;
			virtual void Project(int iterations) override = 0;
			virtual void CollideWithBounds(std::vector<float> &bounds);
			virtual void FinalizeParticles();
			virtual void Transmute() = 0;
			virtual void UpdateAABB();
			virtual std::vector<std::string> GetDebugOutput() override = 0;

			virtual void DeleteParticle(Particles::Particle* particle) override = 0;
			virtual nlohmann::json GetJSON() = 0;

		protected:
			std::unordered_map<int, Particles::Particle*> particleList;
			std::unordered_map<std::string, Forces::Force*> forceList;
			Rendering::GameObjects::Models::Model* model;
			AABB* boundingBox;
		};
	}
}