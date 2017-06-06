#include "StretchingConstraint.h"
#include "../Particles/Particle.h"
#include <glm/glm.hpp>

using namespace Physics;
using namespace Constraints;
using namespace Particles;
using namespace std;
using namespace glm;

StretchingConstraint::StretchingConstraint(Particles::Particle* particle1, Particles::Particle* particle2, float stiffness) :
	StretchingConstraint::StretchingConstraint(particle1, particle2, length(particle1->x0 - particle2->x0), stiffness) { }

StretchingConstraint::StretchingConstraint(Particles::Particle* particle1, Particles::Particle* particle2, float restDistance, float stiffness) {
	ParticleList = vector<Particle*>(2);
	ParticleGradients = vector<vec3>(2);
	ParticleList[0] = particle1;
	ParticleList[1] = particle2;
	ParticleGradients[0] = vec3(0);
	ParticleGradients[1] = vec3(0);
	this->restDistance = restDistance;
	this->stiffness = stiffness;
}

void StretchingConstraint::UpdateDerivs() {
	vec3 n = ParticleList[0]->p - ParticleList[1]->p;
	float distance = length(n);
	normalize(n);
	ParticleGradients[0] = n;
	ParticleGradients[1] = -n;
	this->s = (distance - restDistance) / (ParticleList[0]->w + ParticleList[1]->w);
}

bool StretchingConstraint::ContainsParticle(Particle* particle) {
	return (particle == ParticleList[0] || particle == ParticleList[1]);
}

bool StretchingConstraint::ContainsParticles(Particle* p1, Particle* p2) {
	return (p1 == ParticleList[0] && p2 == ParticleList[1]) || (p2 == ParticleList[0] && p1 == ParticleList[1]);
}