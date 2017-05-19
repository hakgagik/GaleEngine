#include "BoundingForce.h"
#include "../Particles/Particle.h"
#include "../../Managers/Physics_Manager.h"

using namespace Physics;
using namespace Particles;
using namespace Forces;
using namespace Managers;
using namespace std;
using namespace glm;

BoundingForce::BoundingForce(vec3 &direction) {
	this->dir = direction;
}

vec3 BoundingForce::GetForce(Particle* particle) {
	auto kv = affectedParticles.find(particle);
	if (kv == affectedParticles.end()) {
		return vec3(0);
	}
	vec3 v = affectedParticles[particle];
	affectedParticles.erase(particle);
	return -dot(v, dir) * dir * particle->m / Physics_Manager::Get().dt;
	//return vec3(0);
}

void BoundingForce::AddParticle(Particle* particle) {
	affectedParticles[particle] = particle->v;
}