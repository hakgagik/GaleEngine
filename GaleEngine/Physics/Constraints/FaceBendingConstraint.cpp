/// Implemented from "Position Based Dynamics" by Muller, et al
/// http://matthias-mueller-fischer.ch/publications/posBasedDyn.pdf

#include "FaceBendingConstraint.h"
#include "../Particles/Particle.h"

using namespace Physics;
using namespace Constraints;
using namespace Particles;
using namespace std;
using namespace glm;

FaceBendingConstraint::FaceBendingConstraint(Particle* particle1, Particle* particle2, Particle* particle3, Particle* particle4, float stiffness) :
	FaceBendingConstraint(particle1, particle2, particle3, particle4, getAngle(particle1->x0, particle2->x0, particle3->x0, particle4->x0), stiffness) { }

FaceBendingConstraint::FaceBendingConstraint(Particle* particle1, Particle* particle2, Particle* particle3, Particle* particle4, float restAngle, float stiffness) {
	ParticleGradients = unordered_map<Particle*, vec3>();
	this->particle1 = particle1;
	this->particle2 = particle2;
	this->particle3 = particle3;
	this->particle4 = particle4;
	ParticleGradients[particle1] = vec3(0);
	ParticleGradients[particle2] = vec3(0);
	ParticleGradients[particle3] = vec3(0);
	ParticleGradients[particle4] = vec3(0);
	this->restAngle = restAngle;
	this->stiffness = stiffness;
}

inline mat3 getNormalJacobian(vec3 N) {
	float nx = N.x, ny = N.y, nz = N.z;
	float l = length(N);
	return mat3(
		 N.y*N.y + N.z*N.z, -N.x*N.y,           -N.x*N.z,
		-N.x*N.y,            N.x*N.x + N.z*N.z, -N.y*N.z,
		-N.x*N.z,           -N.y*N.z,            N.x*N.x + N.y*N.y
	) / (l*l*l);
}

void FaceBendingConstraint::UpdateDerivs() {
	//vec3 p1, p2, p3, p4, N3, N4, n3, n4;
	//mat3 J3, J4;
	//p1 = particle1->p;
	//p2 = particle2->p;
	//p3 = particle3->p;
	//p4 = particle4->p;
	//float phi = getAngle(p1, p2, p3, p4);
	//float sinPhi = sin(phi);
	//N3 = cross(p2 - p1, p3 - p1);
	//N4 = cross(p2 - p1, p4 - p1);
	//J3 = getNormalJacobian(N3);
	//J4 = getNormalJacobian(N4);
	//n3 = normalize(N3);
	//n4 = normalize(N4);
	//ParticleGradients[particle1] = (J3 * cross(p2 - p3, n4) + J4 * cross(p2 - p4, n3)) / -sinPhi;
	//ParticleGradients[particle2] = (J3 * cross(p3 - p1, n4) + J4 * cross(p4 - p1, n3)) / -sinPhi;
	//ParticleGradients[particle3] = (J3 * cross(p1 - p2, n4)) / -sinPhi;
	//ParticleGradients[particle4] = (J4 * cross(p1 - p2, n3)) / -sinPhi;

	vec3 p1 = vec3(0);
	vec3 p2 = particle2->p - particle1->p;
	vec3 p3 = particle3->p - particle1->p;
	vec3 p4 = particle4->p - particle1->p;

	vec3 n1 = normalize(cross(p2, p3));
	vec3 n2 = normalize(cross(p2, p4));

	float d = dot(n1, n2);
	if (d > 1.0f) d = 1;
	if (d < -1.0f) d = -1;

	float d3 = length(cross(p2, p3))/* * sqrt(1 - d*d)*/;
	float d4 = length(cross(p2, p4))/* * sqrt(1 - d*d)*/;

	ParticleGradients[particle3] = (cross(p2, n2) + cross(n1, p2) * d) / d3;
	ParticleGradients[particle4] = (cross(p2, n1) + cross(n2, p2) * d) / d4;
	ParticleGradients[particle2] = -(cross(p3, n2) + cross(n1, p3) * d) / d3 - (cross(p4, n1) + cross(n2, p4) * d) / d4;
	ParticleGradients[particle1] = -(ParticleGradients[particle2] + ParticleGradients[particle3] + ParticleGradients[particle4]);

	float sum = particle1->w * dot(ParticleGradients[particle1], ParticleGradients[particle1]);
	sum += particle2->w * dot(ParticleGradients[particle2], ParticleGradients[particle2]);
	sum += particle3->w * dot(ParticleGradients[particle3], ParticleGradients[particle3]);
	sum += particle4->w * dot(ParticleGradients[particle4], ParticleGradients[particle4]);

	if (sum > -0.000001f && sum < 0.000001f) {
		this->s = 0;
	}
	else {
		this->s = sqrt(1 - d*d) * (acos(d) - restAngle) / sum;
	}
}

bool FaceBendingConstraint::ContainsParticle(Particle* particle) {
	return (particle == particle1 || particle == particle2 || particle == particle3 || particle == particle4);
}

float FaceBendingConstraint::getAngle(vec3 &p1, vec3 &p2, vec3 &p3, vec3 &p4) {
	vec3 p12 = p2 - p1;
	vec3 p13 = p3 - p1;
	vec3 p14 = p4 - p1;
	vec3 n3 = normalize(cross(p12, p13));
	vec3 n4 = normalize(cross(p12, p14));
	float d = dot(n3, n4);
	if (d > 1.0f) { d = 1.0f; }
	else if (d < -1.0f) { d = -1.0f; }
	return acos(d);
}