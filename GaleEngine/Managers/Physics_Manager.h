#pragma once
#include "../lib/json.hpp"
#include <vector>
#include <unordered_map>

namespace Physics {
	namespace Particles {
		class Particle;
	}
	namespace Constraints {
		class Constraint;
	}
	namespace Forces {
		class Force;
	}
	namespace PhysicsObjects {
		class PhysicsObject;
	}
}

namespace Managers {
	class Physics_Manager {
	public:
		static Physics_Manager& Get();

		int iterations = 10;
		float dt;
		//std::vector<Physics::Particles::Particle*> particleList;
		//std::vector<Physics::Constraints::Constraint*> constraintList;
		//std::vector<Physics::Forces::ForceField*> extForceList;
		std::vector<Physics::PhysicsObjects::PhysicsObject*> PhysicsObjectList;
		std::vector <float> Bounds = { 0, 1, 0, 1, 0, 1 };
		std::unordered_map<std::string, Physics::Forces::Force*> ForceList;

		~Physics_Manager();

		void Init();

		void InitializeParticles();
		void Update();
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
		void applyExtForces();
		void predictPositions();
		void calculatePotentialInteractions();
		void projectConstraints(int iterations);
		void finalizePositionsAndVelocities();

		// Loading methods
		void createCloth(nlohmann::json j);
	};
}