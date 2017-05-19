#include "Physics_Manager.h"
#include "../Physics/Particles/Particle.h"
#include "../Physics/Constraints/Constraint.h"
#include "../Physics/IConstraintGroup.h"
#include "../Physics/PhysicsObjects/PhysicsObject.h"
#include "../Physics/PhysicsObjects/Fluids/FluidHelper.h"
#include <glm/glm.hpp>

using namespace Managers;
using namespace Physics;
using namespace Particles;
using namespace Forces;
using namespace Constraints;
using namespace PhysicsObjects;
using namespace Fluids;
using namespace glm;
using namespace std;
using json = nlohmann::json;

Physics_Manager Physics_Manager::instance;

Physics_Manager& Physics_Manager::Get() {
	return instance;
}

Physics_Manager::Physics_Manager() {}

Physics_Manager::~Physics_Manager() {
	//for (Particle* particle : particleList)	delete particle;
	//for (Constraint* constraint : constraintList) delete constraint;
	//for (ForceField* force : extForceList) delete force;
	for (PhysicsObject* physicsObject : PhysicsObjectList) delete physicsObject;
}

void Physics_Manager::Init() {
	FluidHelper::Init();
}

void Physics_Manager::InitializeParticles() {
	initParticles();
}

void Physics_Manager::Update() {
	applyExtForces();
	predictPositions();
	FluidHelper::Get().Update();
	calculatePotentialInteractions();
	for (int i = 0; i < iterations; i++) {
		projectConstraints(iterations);
	}
	finalizePositionsAndVelocities();
}

void Physics_Manager::Transmute() {
	for (PhysicsObject* physObj : PhysicsObjectList) {
		physObj->Transmute();
	}
}

void Physics_Manager::initParticles() {
	for (PhysicsObject* physObj : PhysicsObjectList) {
		physObj->InitParticles();
	}
}

void Physics_Manager::applyExtForces() {
	for (PhysicsObject* physObj : PhysicsObjectList) {
		physObj->ApplyForces();
	}
}

void Physics_Manager::predictPositions() {
	for (PhysicsObject* physObj : PhysicsObjectList) {
		physObj->PredictPositions();
	}
}

void Physics_Manager::calculatePotentialInteractions() {
	//TODO
	for (PhysicsObject* physObj : PhysicsObjectList) {
		physObj->CalculatePotentialInteractions();
	}
}

void Physics_Manager::projectConstraints(int iterations) {
	for (PhysicsObject* physObj : PhysicsObjectList) {
		physObj->Project(iterations);
		physObj->CollideWithBounds(Bounds);
	}
}

void Physics_Manager::finalizePositionsAndVelocities() {
	for (PhysicsObject* physObj : PhysicsObjectList) {
		physObj->FinalizeParticles();
	}
}

void Physics_Manager::AddPhysicsObject(PhysicsObject* physicsObject) {
	PhysicsObjectList.push_back(physicsObject);
}

void Physics_Manager::LoadFromJSON(json &j) {
	for (json t : j["Physics"]["PhysicsObjects"]) {
		string objectType =  t["Type"];
		if (objectType == "Cloth") {
			createCloth(t);
		}
	}

	//for (json t : j["Physics"]["Forces"]) {

	//}
}

void Physics_Manager::WriteToJSON(json &j) {
	for (PhysicsObject* physObj : PhysicsObjectList) {
		j["Physics"]["PhysicsObjects"].push_back(physObj->GetJSON());
	}
	//for (ForceField* forceField : extForceList) {
	//	j["Physics"]["Forces"].push_back(forceField->GetJSON());
	//}
}

void Physics_Manager::createCloth(json j) {

}