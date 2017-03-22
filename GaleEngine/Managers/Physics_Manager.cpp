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
	for (PhysicsObject* physicsObject : physicsObjectList) delete physicsObject;
}

void Physics_Manager::Init() { }

void Physics_Manager::InitializeParticles() {
	initParticles();
}

void Physics_Manager::Update(float dt) {
	applyExtForces(dt);
	predictPositions(dt);
	FluidHelper::Get().Update();
	genCollConstraints();
	for (int i = 0; i < iterations; i++) {
		projectConstraints(iterations);
	}
	finalizePositionsAndVelocities(dt);
}

void Physics_Manager::Transmute() {
	for (PhysicsObject* physObj : physicsObjectList) {
		physObj->Transmute();
	}
}

void Physics_Manager::initParticles() {
	for (PhysicsObject* physObj : physicsObjectList) {
		physObj->InitParticles();
	}
}

void Physics_Manager::applyExtForces(float dt) {
	for (PhysicsObject* physObj : physicsObjectList) {
		physObj->ApplyForces(dt);
	}
}

void Physics_Manager::predictPositions(float dt) {
	for (PhysicsObject* physObj : physicsObjectList) {
		physObj->PredictPositions(dt);
	}
}

void Physics_Manager::genCollConstraints() {
	//TODO
	for (PhysicsObject* physObj : physicsObjectList) {
		physObj->GenerateConstraints(dt);
	}
}

void Physics_Manager::projectConstraints(int iterations) {
	for (PhysicsObject* physObj : physicsObjectList) {
		physObj->Project(iterations);
		physObj->CollideWithBounds(bounds);
	}
}

void Physics_Manager::finalizePositionsAndVelocities(float dt) {
	for (PhysicsObject* physObj : physicsObjectList) {
		physObj->FinalizeParticles(dt);
	}
}

void Physics_Manager::AddPhysicsObject(PhysicsObject* physicsObject) {
	physicsObjectList.push_back(physicsObject);
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
	for (PhysicsObject* physObj : physicsObjectList) {
		j["Physics"]["PhysicsObjects"].push_back(physObj->GetJSON());
	}
	//for (ForceField* forceField : extForceList) {
	//	j["Physics"]["Forces"].push_back(forceField->GetJSON());
	//}
}

void Physics_Manager::createCloth(json j) {

}