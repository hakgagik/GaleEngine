#include "DensityConstraint.h"
#include "../Particles/Particle.h"
#include "../PhysicsObjects/Fluids/FluidHelper.h"
#include <glm/gtc/constants.hpp>

using namespace Physics;
using namespace Constraints;
using namespace Particles;
using namespace PhysicsObjects::Fluids;
using namespace glm;

float DensityConstraint::h = 0.1f;
float DensityConstraint::epsilon = 600.0f;
float DensityConstraint::poly6Factor = 315.0f / (64.0f * pi<float>() * h * h * h * h * h * h);
float DensityConstraint::spikyFactor = 15.0f / (pi<float>() * h * h * h * h);
float DensityConstraint::delSpikyFactor = -3 * spikyFactor;
float DensityConstraint::k = 0.1f;
float DensityConstraint::n = 4;
float DensityConstraint::q = 0.2f * h;
float DensityConstraint::sCorrDenom = DensityConstraint::Poly6Kernel(q);
float DensityConstraint::c = 0.01f;

//void DensityConstraint::SetH(float newH) {
//	h = newH;
//	poly6Factor = 315.0f / (64.0f * pi<float>() * h * h * h * h * h * h);
//	spikyFactor = 15.0f / (pi<float>() * h * h * h * h);
//	delSpikyFactor = -3 * spikyFactor;
//	sCorrDenom = Poly6Kernel(q);
//}

float DensityConstraint::Poly6Kernel(float &r) {
	if (r < h) {
		float temp = h * h - r * r;
		return poly6Factor * temp * temp * temp;
	}
	return 0;
}

float DensityConstraint::SpikeyKernel(float &r) {
	if (r < h) {
		float temp = h - r;
		return spikyFactor * temp * temp * temp;
	}
	return 0;
}

float DensityConstraint::DelSpikeyKernel(float &r) {
	if (r < h) {
		float temp = h - r;
		return delSpikyFactor * temp * temp;
	}
	return 0;
}

DensityConstraint::DensityConstraint(Particle* center, float restDensity) {
	this->Center = center;
	this->RestDensity = restDensity;
}

void DensityConstraint::FindNeighbors(FluidHelper &fluidHelper) {
	ParticleGradients.clear();
	int xBin = (int)round(Center->x.x / h);
	int yBin = (int)round(Center->x.y / h);
	int zBin = (int)round(Center->x.z / h);

	ivec3 neighborInds;
	for (int k = -1; k <= 1; k++) { // Iterate over z bins
		neighborInds[2] = zBin + k;
		for (int j = -1; j <= 1; j++) { // Iterate over y bins
			neighborInds[1] = yBin + j;
			for (int i = -1; i <= 1; i++) { // Iterate over x bins
				neighborInds[0] = xBin + i; 
				auto bin = fluidHelper.Bins.find(neighborInds); // Check if the bin exists
				if (bin != fluidHelper.Bins.end()) {
					for (Particle* pOther : bin->second) {
						if (pOther != Center && length(pOther->x - Center->x) < h) {
							ParticleGradients[pOther]=vec3(0);
						}
					}
				}
			}
		}
	}

}

void DensityConstraint::CalculateOnlyDensity() {
	CurrentDensity = 0;
	for (auto kv : ParticleGradients) {
		float r = length(kv.first->p - Center->p);
		if (r > h) continue;
		CurrentDensity += kv.first->m * Poly6Kernel(r);
	}
}

void DensityConstraint::UpdateDerivs() {
	CurrentDensity = 0;
	Lambda = 0;
	CenterDeriv = vec3(0);

	
	for (auto kv : ParticleGradients) {
		vec3 r_hat = kv.first->p - Center->p;
		float r = length(r_hat);
		r_hat = normalize(r_hat);
		if (r > h) continue;
		CurrentDensity += kv.first->m * Poly6Kernel(r);
		float derivFactor = DelSpikeyKernel(r);
		CenterDeriv += kv.first->m * r_hat * derivFactor;
		Lambda += Center->m * Center->m * derivFactor * derivFactor;
		kv.second = r_hat * derivFactor;
	}

	Lambda += dot(CenterDeriv, CenterDeriv);
	Lambda /= RestDensity * RestDensity;
	Lambda += epsilon;
	Lambda = (CurrentDensity / RestDensity - 1) / Lambda;
}

bool DensityConstraint::ContainsParticle(Particle* particle) {
	for (auto kv : ParticleGradients) {
		if (kv.first == particle) {
			return true;
		}
	}
	return false;
}

void DensityConstraint::SetRestDensity(float newDensity) {
	RestDensity = newDensity;
}

vec3 DensityConstraint::GetDP() {
	vec3 dp(0);
	if (Center->w == 0) return dp;
	for (auto kv : ParticleGradients) {
		DensityConstraint* otherConstriant = FluidHelper::Get().FluidParticles[kv.first];
		float r = length(Center->p - otherConstriant->Center->p);
		float s_corr = Poly6Kernel(r) / sCorrDenom;
		s_corr = -k * pow(s_corr, n);
		dp += (Center->m * otherConstriant->Lambda + Lambda * otherConstriant->Center->m + s_corr) * kv.second;
	}
	return (Center->w / RestDensity) * dp;
}

vec3 DensityConstraint::GetDV() {
	vec3 dv(0);
	for (auto kv : ParticleGradients) {
		Particle* other = kv.first;
		float r = length(other->p - Center->p);
		if (r > h) continue;
		dv += Poly6Kernel(r) * other->m * (other->p - Center->p);
	}
	return dv * c;
}

vec3 DensityConstraint::GetFVC() {
	return vec3(0);
}