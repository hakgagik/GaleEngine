#include "PhysicsObject.h"
#include "../Particles/Particle.h"
#include "../Forces/Force.h"
#include <glm/glm.hpp>

using namespace Physics;
using namespace Particles;
using namespace PhysicsObjects;
using namespace Forces;
using namespace glm;

void PhysicsObject::AddForce(Force* force) {
	this->forceList.push_back(force);
}

void PhysicsObject::InitParticles() {
	for (auto kv : particleList) {
		Particle* particle = kv.second;
		particle->x = particle->x0;
		particle->v = particle->v0;
		particle->w = 1.0f / particle->m;
	}
}

void PhysicsObject::ApplyForces(float dt) {
	for (auto kv : particleList) {
		Particle* particle = kv.second;
		vec3 forceAccum;
		for (Force* force : forceList) {
			forceAccum += force->GetForce(particle);
		}
		particle->v += forceAccum * particle->w * dt;
	}
}

void PhysicsObject::PredictPositions(float dt) {
	for (auto kv : particleList) {
		Particle* particle = kv.second;
		particle->p += dt * particle->v;
	}
}

void PhysicsObject::GenerateConstraints(float dt) { }

void PhysicsObject::FinalizeParticles(float dt) {
	for (auto kv : particleList) {
		Particle* particle = kv.second;
		particle->v = (particle->p - particle->x) / dt;
		particle->x = particle->p;
	}
}