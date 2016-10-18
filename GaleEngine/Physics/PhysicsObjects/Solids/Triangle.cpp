#include "Triangle.h"

using namespace Physics;
using namespace PhysicsObjects;
using namespace Solids;
using namespace Particles;

Triangle::Triangle(Particle* p1, Particle* p2, Particle* p3) {
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}

Particle* Triangle::GetOther(Particle* pa, Particle* pb) {
	if (pa == p1 && pb == p2 || pb == p1 && pa == p2) return p3;
	else if (pa == p2 && pb == p3 || pb == p2 && pa == p3) return p1;
	else if (pa == p3 && pb == p1 || pb == p3 && pa == p1) return p2;
	else return nullptr;
}
