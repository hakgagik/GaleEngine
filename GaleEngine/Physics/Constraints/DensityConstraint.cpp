#include "DensityConstraint.h"
#include "../Particles/Particle.h"
#include "../PhysicsObjects/Fluids/FluidHelper.h"
#include <glm/gtc/constants.hpp>
#include <vector>

using namespace Physics;
using namespace Constraints;
using namespace Particles;
using namespace PhysicsObjects::Fluids;
using namespace glm;

float DensityConstraint::h = 0.1f;
float DensityConstraint::h_sq = DensityConstraint::h * DensityConstraint::h;
float DensityConstraint::epsilon = 600.0f;
float DensityConstraint::poly6Factor = 315.0f / (64.0f * pi<float>() * h * h * h * h * h * h * h * h * h);
float DensityConstraint::spikyFactor = 15.0f / (pi<float>() * h * h * h * h * h * h);
float DensityConstraint::delSpikyFactor = -3 * spikyFactor;
float DensityConstraint::k = 0.0001f;
//unsigned int DensityConstraint::n = 4;
float DensityConstraint::q = 0.3f * h;
float DensityConstraint::sCorrDenom = DensityConstraint::Poly6Kernel(q);
float DensityConstraint::c = 0.02f;

//void DensityConstraint::SetH(float newH) {
//	h = newH;
//	poly6Factor = 315.0f / (64.0f * pi<float>() * h * h * h * h * h * h);
//	spikyFactor = 15.0f / (pi<float>() * h * h * h * h);
//	delSpikyFactor = -3 * spikyFactor;
//	sCorrDenom = Poly6Kernel(q);
//}

float ln_sq(vec3 &r) {
	return r.x * r.x + r.y * r.y + r.z * r.z;
}

float DensityConstraint::Poly6Kernel(float &r) {
	if (r < h) {
		float temp = h_sq - r * r;
		return poly6Factor * temp * temp * temp;
	}
	return 0;
}

float DensityConstraint::Poly6Kernel_rsq(float &r_sq) {
	if (r_sq < h_sq) {
		float temp = h_sq - r_sq;
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

DensityConstraint::DensityConstraint(Particle* center, float restDensity, float stiffness) {
	this->Center = center;
	this->RestDensity = restDensity;
	this->stiffness = stiffness;
}

void DensityConstraint::FindNeighbors(FluidHelper &fluidHelper) {
	ParticleList.clear();

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
						if (pOther != Center && ln_sq(pOther->x - Center->x) < h_sq) {
							//ParticleGradients[pOther];
							ParticleList.push_back(pOther);
						}
					}
				}
			}
		}
	}
	ParticleGradients = std::vector<vec3>(ParticleList.size());
}

float DensityConstraint::CalculateLocalDensity() {
	float localDensity = 0.0f;
	localDensity = Poly6Kernel(localDensity) * Center->m;
	for (int i = 0; i < ParticleList.size(); i++) {
		float r_sq = ln_sq(ParticleList[i]->p - Center->p);
		if (r_sq > h_sq) continue;
		localDensity += ParticleList[i]->m * Poly6Kernel_rsq(r_sq);
	}
	return localDensity;
}

//#pragma optimize("", off)
void DensityConstraint::UpdateDerivs() {
  	CurrentDensity = 0;
	CurrentDensity = Poly6Kernel(CurrentDensity) * Center->m;
	Lambda = 0.0;
	CenterDeriv = vec3(0);

	for (int i = 0; i < ParticleList.size(); i++) {
		Particle* pOther = ParticleList[i];
		vec3 r_hat = Center->p - pOther->p;
		float r = length(r_hat);
		if (r > h) continue;
		if (r != 0) r_hat /= r;
		CurrentDensity += pOther->m * Poly6Kernel(r);
		float derivFactor = DelSpikeyKernel(r);
		CenterDeriv += pOther->m * r_hat * derivFactor;
		Lambda += Center->m * Center->m * derivFactor * derivFactor;
		ParticleGradients[i] = r_hat * derivFactor;
	}

	Lambda += dot(CenterDeriv, CenterDeriv);
	Lambda /= RestDensity * RestDensity;
	Lambda += epsilon;
	Lambda = -(CurrentDensity / RestDensity - 1) / Lambda;
}
//#pragma optimize("", on)

bool DensityConstraint::ContainsParticle(Particle* particle) {
	for (Particle* pOther : ParticleList) {
		if (pOther == particle) {
			return true;
		}
	}
	return false;
}

void DensityConstraint::SetRestDensity(float newDensity) {
	RestDensity = newDensity;
}

glm::vec3 DensityConstraint::GetDP() {
	vec3 dp(0);
	if (Center->w == 0) return dp;
	FluidHelper& fluidHelper = FluidHelper::Get();
	for (int i = 0; i < ParticleList.size(); i++) {
		DensityConstraint* otherConstriant = fluidHelper.FluidParticles[ParticleList[i]];
		//DensityConstraint* otherConstriant = FluidHelper::Get().FluidParticles[kv.first];
		float r_sq = ln_sq(otherConstriant->Center->p - Center->p);
		if (r_sq > h_sq || r_sq <= 0) continue;
		float s_corr = Poly6Kernel_rsq(r_sq) / sCorrDenom;
		s_corr = -k * s_corr * s_corr * s_corr * s_corr;
		float dpFactor = (Center->m * otherConstriant->Lambda + Lambda * otherConstriant->Center->m + s_corr);
		dp += ParticleGradients[i] * dpFactor;
	}
	dp *= (Center->w / RestDensity);
	return dp;
}

vec3 DensityConstraint::GetDV() {
	vec3 dv(0);
	for (int i = 0; i < ParticleList.size(); i++) {
		Particle* pOther = ParticleList[i];
		float r = length(Center->p - pOther->p);
		if (r > h) continue;
		dv += Poly6Kernel(r) * pOther->m * (pOther->p - Center->p) / FluidHelper::Get().FluidParticles[pOther]->CalculateLocalDensity();
	}
	return dv * c;
}

vec3 DensityConstraint::GetFVC() {
	return vec3(0);
}