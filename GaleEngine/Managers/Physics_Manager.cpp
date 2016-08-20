#include "Physics_Manager.h"
#include "../Physics/Particles/Particle.h"
#include "../Physics/Forces/ForceField.h"
#include "../Physics/Constraints/Constraint.h"
#include "../Physics/Constraints/ConstraintGroup.h"
#include "../Physics/Transmuters/Transmuter.h"
#include <glm/glm.hpp>

using namespace Managers;
using namespace Physics;
using namespace Particles;
using namespace Forces;
using namespace Constraints;
using namespace Transmuters;
using namespace glm;
using namespace std;
using json = nlohmann::json;

Physics_Manager::Physics_Manager() {}
Physics_Manager::~Physics_Manager() {
	for (Particle* particle : particleList)	delete particle;
	for (Constraint* constraint : constraintList) delete constraint;
	for (ForceField* force : extForceList) delete force;
	for (Transmuter* transmuter : transmuterList) delete transmuter;
}

void Physics_Manager::Update(float dt) {
	applyExtForces(dt);
	predictPositions(dt);
	genCollConstraints();
	for (int i = 0; i < iterations; i++) {
		projectConstraints(dt, iterations);
	}
	finalizePositionsAndVelocities(dt);
	velocityUpdate();
}

void Physics_Manager::Transmute() {
	for (Transmuter* transmuter : transmuterList) {
		transmuter->Transmute();
	}
}

void Physics_Manager::initParticles() {
	for (Particle* particle : particleList) {
		particle->x = particle->x0;
		particle->v = particle->v0;
		particle->w = 1.0f / particle->m;
	}
}

void Physics_Manager::applyExtForces(float dt) {
	for (Particle* particle : particleList) {
		vec3 forceAccum(0);
		for (ForceField* force : particle->affectingForces) {
			forceAccum += force->GetForce(particle->x, particle->v);
		}
		particle->v += dt*particle->w * forceAccum;
	}
}

void Physics_Manager::predictPositions(float dt) {
	for (Particle* particle : particleList) {
		particle->x += dt * particle->v;
	}
}

void Physics_Manager::genCollConstraints() {

}

void Physics_Manager::projectConstraints(float dt, int iterations) {
	for (Constraint* constraint : constraintList) {
		constraint->Update();
		float iter_stiffness = 1.0f - pow(1.0f - constraint->stiffness, 1.0f / (float)iterations);
		for (auto pg_pair : constraint->particleGradients) {
			pg_pair.first->p += -iter_stiffness * constraint->lambda * pg_pair.first->w * pg_pair.second;
		}
	}
}

void Physics_Manager::finalizePositionsAndVelocities(float dt) {
	for (Particle* particle : particleList) {
		particle->v = (particle->p - particle->x) / dt;
		particle->x = particle->v;
	}
}

void Physics_Manager::velocityUpdate() {

}

void Physics_Manager::LoadFromJSON(json &j) {
	for (json t : j["Physics"]["Transmuters"]) {
		string transmuterType =  t["Type"];
		if (transmuterType == "Cloth") {
			createCloth(t);
		}
	}

	for (json t : j["Physics"]["Forces"]) {

	}
}

void Physics_Manager::WriteToJSON(json &j) {
	for (Transmuter* transmuter : transmuterList) {
		j["Physics"]["Transmuters"].push_back(transmuter->GetJSON());
	}
	for (ForceField* forceField : extForceList) {
		j["Physics"]["Forces"].push_back(forceField->GetJSON());
	}
}

void Physics_Manager::createCloth(json j) {

}