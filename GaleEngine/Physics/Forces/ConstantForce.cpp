#include "ConstantForce.h"
#include "../Particles/Particle.h"

using namespace Physics;
using namespace Forces;
using namespace Particles;
using namespace glm;

ConstantForce::ConstantForce(vec3 &acceleration) {
	this->acc = acceleration;
}

vec3 ConstantForce::GetForce(Particle* particle) {
	return acc * particle->m;
}