#include "FluidHelper.h"
#include "../../Particles/Particle.h"
#include "../../Constraints/DensityConstraint.h"

#include <iostream>

using namespace Physics;
using namespace Particles;
using namespace PhysicsObjects;
using namespace Fluids;
using namespace Constraints;
using namespace std;

FluidHelper FluidHelper::instance;

void FluidHelper::Init() { }

FluidHelper::FluidHelper() { }

FluidHelper::~FluidHelper() { }

FluidHelper& FluidHelper::Get() {
	return instance;
}

FluidHelper FluidHelper::GetThrowaway() {
	return FluidHelper();
}

void FluidHelper::AddParticle(DensityConstraint* constraint) {
	FluidParticles[constraint->Center] = constraint;
}

void FluidHelper::AddParticles(vector<DensityConstraint*> &constraints) {
	for (DensityConstraint* c : constraints) {
		AddParticle(c);
	}
}

void FluidHelper::DeleteParticle(Particle* particle) {
	FluidParticles.erase(particle);
}

void FluidHelper::Update() {
	// TODO: Add logic for a maximum number of bins. We don't want to run out of memory because something escaped the system.
	for (auto kv : Bins) {
		Bins[kv.first].clear();
	}

	for (auto kv : FluidParticles) {
		Particle* p = kv.first;
		int x = (int)floor(p->p.x / h);
		int y = (int)floor(p->p.y / h);
		int z = (int)floor(p->p.z / h);
		Bins[{x, y, z}].push_back(p);
	}
}

void FluidHelper::ReBin(float newH) {
	this->h = newH;
	Update();
}

void FluidHelper::Clear() {
	FluidParticles.clear();
	for (auto kv : Bins) {
		kv.second.clear();
	}
}