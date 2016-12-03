#include "DensityConstraint.h"
#include <glm/gtc/constants.hpp>

using namespace Physics;
using namespace Constraints;
using namespace Particles;
using namespace glm;

vec3 DensityConstraint::h = { 0.1f, 0.01f, 0.001f };
float DensityConstraint::poly6Factor = 315.0 / (64.0 * pi<float>() * h[2] * h[2] * h[2]);
float DensityConstraint::spikyFactor = 15.0 / (pi<float>() * h[2] * h[2]);
float DensityConstraint::delSpikyFactor = -3 * spikyFactor;

float DensityConstraint::SetH(float newH) {
	h = { newH, newH * newH, newH * newH * newH };
	poly6Factor = 315.0 / (64.0 * pi<float>() * h[2] * h[2] * h[2]);
	spikyFactor = 15.0 / (pi<float>() * h[2] * h[2]);
	delSpikyFactor = -3 * spikyFactor;
}

float DensityConstraint::Poly6Kernel(float r) {
	if (r < h[0]) {
		double temp = h[1] - r * r;
		return poly6Factor * temp * temp * temp;
	}
	else return 0;
}

float DensityConstraint::SpikeyKernel(float r) {
	if (r < h[0]) {
		double temp = h[0] - r;
		return spikyFactor * temp * temp * temp;
	}
	else return 0;
}

float DensityConstraint::DelSpikeyKernel(float r) {
	if (r < h[0]) {
		double temp = h[0] - r;
		return delSpikyFactor * temp * temp;
	}
	else return 0;
}

DensityConstraint::DensityConstraint(Particle* center, float density) {
	this->center = center;
	this->density = density;
}

void DensityConstraint::UpdateDerivs() {
	for (auto kv : particleGradients) {
		if (kv.first == center) {
			// TODO
		}
		else {

		}
	}
}

bool DensityConstraint::ContainsParticle(Particle* particle) {
	for (auto kv : particleGradients) {
		if (kv.first == particle) {
			return true;
		}
	}
	return false;
}