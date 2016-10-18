#include "Particle.h"

using namespace Physics;
using namespace Particles;
using namespace glm;

Particle::Particle() {}
Particle::Particle(vec3 pos, vec3 vel, float mass) {
	this->x = this->x0 = this->p = pos;
	this->v = this->v0 = vel;
	this->m = mass;
	this->w = 1.0f / mass;
	this->isFixed = false;
}

Particle::~Particle() {}

void Particle::Fix() {
	this->isFixed = true;
	this->w = 0;
}

void Particle::Unfix() {
	this->isFixed = false;
	this->w = 1.0f / this->m;
}