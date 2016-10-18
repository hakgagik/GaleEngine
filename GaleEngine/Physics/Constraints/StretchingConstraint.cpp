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
	particleGradients = unordered_map<Particle*, vec3>();
	this->particle1 = particle1;
	this->particle2 = particle2;
	particleGradients[particle1] = vec3(0);
	particleGradients[particle2] = vec3(0);
	this->restDistance = restDistance;
	this->stiffness = stiffness;
}

void StretchingConstraint::UpdateDerivs() {
	vec3 n = particle1->p - particle2->p;
	float distance = length(n);
	normalize(n);
	particleGradients[particle1] = n;
	particleGradients[particle2] = -n;
	this->s = (distance - restDistance) / (particle1->w + particle2->w);
}

bool StretchingConstraint::ContainsParticle(Particle* particle) {
	return (particle == particle1 || particle == particle2);
}

bool StretchingConstraint::ContainsParticles(Particle* p1, Particle* p2) {
	return (p1 == particle1 && p2 == particle2) || (p2 == particle1 && p1 == particle2);
}