#include "StretchingConstraintGroup.h"
#include "StretchingConstraint.h"

using namespace Physics;
using namespace Constraints;
using namespace Particles;
using namespace std;

void StretchingConstraintGroup::Update() {
	// TODO: figure out if we can optimise this to take fixed particles into account
	for (StretchingConstraint* constraint : constraints) {
		constraint->Update();
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