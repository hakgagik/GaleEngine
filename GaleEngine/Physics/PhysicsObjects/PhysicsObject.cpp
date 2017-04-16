#include "PhysicsObject.h"
#include "../Particles/Particle.h"
#include "../Forces/Force.h"
#include "../AABB.h"
#include <glm/glm.hpp>

using namespace Physics;
using namespace Particles;
using namespace PhysicsObjects;
using namespace Forces;
using namespace glm;
using namespace std;

PhysicsObject::~PhysicsObject() { }

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

void PhysicsObject::CollideWithBounds(vector<float> &bounds) {
	float xmin = bounds[0];
	float xmax = bounds[1];
	float ymin = bounds[2];
	float ymax = bounds[3];
	float zmin = bounds[4];
	float zmax = bounds[5];

	for (auto kv : particleList) {
		vec3 &p = kv.second->p;
		if (p.x < xmin)
			p.x = xmin + xmin - p.x;
		if (p.x > xmax) 
			p.x = xmax + xmax - p.x;
		if (p.y < ymin) 
			p.y = ymin + ymin - p.y;
		if (p.y > ymax) 
			p.y = ymax + ymax - p.y;
		if (p.z < zmin) 
			p.z = zmin + zmin - p.z;
		if (p.z > zmax) 
			p.z = zmax + zmax - p.z;
	}
}

void PhysicsObject::FinalizeParticles(float dt) {
	for (auto kv : particleList) {
		Particle* particle = kv.second;
		particle->v = (particle->p - particle->x) / dt;
		particle->x = particle->p;
	}
}

void PhysicsObject::UpdateAABB() {
	vec3 p = particleList.begin()->second->x;
	boundingBox->xmin = boundingBox->xmax = p.x;
	boundingBox->ymin = boundingBox->ymax = p.y;
	boundingBox->zmin = boundingBox->zmax = p.z;

	for (auto kv : particleList) {
		p = kv.second->x;
		if (p.x < boundingBox->xmin) {
			boundingBox->xmin = p.x;
		}
		else if (p.x > boundingBox->xmax) {
			boundingBox->xmax = p.x;
		}
		if (p.y < boundingBox->ymin) {
			boundingBox->ymin = p.y;
		}
		else if (p.y > boundingBox->ymax) {
			boundingBox->ymax = p.y;
		}
		if (p.z < boundingBox->zmin) {
			boundingBox->zmin = p.z;
		}
		else if (p.z > boundingBox->zmax) {
			boundingBox->zmax = p.z;
		}
	}
}