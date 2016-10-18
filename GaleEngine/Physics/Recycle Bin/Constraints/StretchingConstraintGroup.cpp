#include "StretchingConstraintGroup.h"
#include "StretchingConstraint.h"
#include "../Particles/Particle.h"

using namespace Physics;
using namespace Constraints;
using namespace Particles;
using namespace std;

void StretchingConstraintGroup::Project(int iterations) {
	// TODO: figure out if we can optimise this to take fixed particles into account
	for (StretchingConstraint* constraint : constraints) {
		constraint->UpdateDerivs();
	}
	for (StretchingConstraint* constraint : constraints) {
		float iter_stiffness = 1.0f - pow(1.0f - constraint->stiffness, 1.0f / (float)iterations);
		for (auto pg_pair : constraint->particleGradients) {
			pg_pair.first->p += -iter_stiffness * constraint->s * pg_pair.first->w * pg_pair.second;
		}
	}
}

void StretchingConstraintGroup::DeleteParticle(Particle* particle) {
	vector<StretchingConstraint*>::iterator it = constraints.begin();
	while (it != constraints.end()) {
		if ((*it)->ContainsParticle(particle)) {
			delete (*it);
			it = constraints.erase(it);
		} else {
			++it;
		}
	}
}

void StretchingConstraintGroup::CreateStretchingConstraint(Particle* particle1, Particle* particle2, float distance) {
	constraints.push_back(new StretchingConstraint(particle1, particle2, distance));
}

void StretchingConstraintGroup::CreateStretchingConstraint(Particle* particle1, Particle* particle2) {
	constraints.push_back(new StretchingConstraint(particle1, particle1));
}