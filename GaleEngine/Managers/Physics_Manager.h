#pragma once
#include "../lib/json.hpp"
#include <vector>

namespace Physics {
	namespace Particles {
		class Particle;
	}
	namespace Constraints {
		class Constraint;
		class ConstraintGroup;
	}
	namespace Forces {
		class ForceField;
	}
	namespace Transmuters {
		class Transmuter;
	}
}

namespace Managers {
	class Physics_Manager {
	public:
		int iterations = 10;
		float dt;
		std::vector<Physics::Particles::Particle*> particleList;
		std::vector<Physics::Constraints::ConstraintGroup*> constraintGroupList;
		std::vector<Physics::Constraints::Constraint*> constraintList;
		std::vector<Physics::Forces::ForceField*> extForceList;
		std::vector<Physics::Transmuters::Transmuter*> transmuterList;

		Physics_Manager();
		~Physics_Manager();

		void Update(float dt);
		void Transmute();
		void LoadFromJSON(nlohmann::json &j);
		void WriteToJSON(nlohmann::json &j);
	private:
		// Main physics loop functions
		void initParticles();
		void applyExtForces(float dt);
		void predictPositions(float dt);
		void genCollConstraints();
		void projectConstraints(float dt, int iterations);
		void finalizePositionsAndVelocities(float dt);
		void velocityUpdate();

		// Loading methods
		void createCloth(nlohmann::json j);
	};
}