#pragma once
#include "../lib/json.hpp"
#include <vector>

namespace Physics {
	namespace Particles {
		class Particle;
	}
	namespace Constraints {
		class Constraint;
	}
	namespace Forces {
		class ForceField;
	}
	namespace PhysicsObjects {
		class PhysicsObject;
	}
}

namespace Managers {
	class Physics_Manager {
	public:
		static Physics_Manager& Get();

		int iterations = 5;
		float dt;
		//std::vector<Physics::Particles::Particle*> particleList;
		//std::vector<Physics::Constraints::Constraint*> constraintList;
		//std::vector<Physics::Forces::ForceField*> extForceList;
		std::vector<Physics::PhysicsObjects::PhysicsObject*> physicsObjectList;
		std::vector <float> bounds = { 0, 10, 0, 10, 0, 10 };

		~Physics_Manager();

		void Init();

		void InitializeParticles();
		void Update(float dt);
		void Transmute();

		void AddPhysicsObject(Physics::PhysicsObjects::PhysicsObject* physicsObject);
		void LoadFromJSON(nlohmann::json &j);
		void WriteToJSON(nlohmann::json &j);
	private:
		static Physics_Manager instance;

		Physics_Manager();
		Physics_Manager(const Physics_Manager &other);

		// Main physics loop functions
		void initParticles();
		void applyExtForces(float dt);
		void predictPositions(float dt);
		void genCollConstraints();
		void projectConstraints(int iterations);
		void finalizePositionsAndVelocities(float dt);

		// Loading methods
		void createCloth(nlohmann::json j);
	};
}