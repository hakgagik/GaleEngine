#include "Fluid.h"
#include "FluidHelper.h"
#include "../../Particles/Particle.h"
#include "../../Constraints/DensityConstraint.h"
#include "../../../Rendering/GameObjects/Models/Model.h"
#include "../../../Rendering/GameObjects/Models/Fragment.h"
#include "../../../Rendering/Materials/SphereFluidMaterial.h"
#include <unordered_map>

using namespace Physics;
using namespace Particles;
using namespace Constraints;
using namespace PhysicsObjects::Fluids;
using namespace Rendering::GameObjects::Models;
using namespace Rendering::Materials;
using namespace std;
using namespace glm;
using json = nlohmann::json;

Fluid::Fluid() : PhysicsObject() {}

Fluid::Fluid(Model* model, vector<vec3> &positions, float particleMass, float restDensity, float densityStiffness)
{
	// TODO: populate particleList


	this->model = model;
	this->restDensity = restDensity;

	for (vec3 pos : positions) {
		Particle* p = new Particle(pos, vec3(0), particleMass);
		DensityConstraint* c = new DensityConstraint(p, this->restDensity);
		densityConstraintList.push_back(c);
	}

	if (restDensity < 0) {
		FluidHelper tempFluidHelper = FluidHelper::GetThrowaway();
		tempFluidHelper.AddParticles(densityConstraintList);

		tempFluidHelper.Update();
		unordered_map<float, int> densityCounts;
		for (DensityConstraint* c : densityConstraintList) {
			c->FindNeighbors(tempFluidHelper);
			c->CalculateOnlyDensity();
			densityCounts[c->CurrentDensity]++;
		}

		float bestDensity = 0;
		int highestCount = 0;
		for (auto kv : densityCounts) {
			if (kv.second > highestCount) {
				bestDensity = kv.first;
				highestCount = kv.second;
			}
		}

		this->restDensity = bestDensity; // Rest density is Best density

		for (DensityConstraint* c : densityConstraintList) {
			c->SetRestDensity(this->restDensity);
		}
	}

	FluidHelper::Get().AddParticles(densityConstraintList);

}

Fluid::~Fluid() {
	for (DensityConstraint* c : densityConstraintList) {
		delete c;
	}
}

void Fluid::Project(int iterations) {
	for (DensityConstraint* constraint : densityConstraintList) {
		vec3 dp;
		float iter_stiffness = 1.0f - pow(1.0f - constraint->stiffness, 1.0f / (float)iterations);
		for (auto kv : constraint->ParticleGradients) {
			constraint->Center->p += iter_stiffness * constraint->GetDP();
		}
	}
}

void Fluid::FinalizeParticles(float dt) {
	for (auto kv : particleList) {
		kv.second->v = (kv.second->p - kv.second->x) / dt;
	}

	for (DensityConstraint* constraint : densityConstraintList) {
		constraint->Center->v += constraint->GetFVC() * constraint->Center->w * dt;
	}

	//TODO: this looks like it's time step dependant. Make sure it's not
	for (DensityConstraint* constraint : densityConstraintList) {
		constraint->Center->v += constraint->GetDV() * constraint->Center->w;
	}

	for (auto kv : particleList) {
		kv.second->x = kv.second->p;
	}
}

void Fluid::DeleteParticle(Particle* particle) {
	//TODO: use events to trigger this
	auto s = densityConstraintList.begin();
	while (s != densityConstraintList.end()) {
		if ((*s)->Center == particle) {
			delete (*s);
			s = densityConstraintList.erase(s);
		}
		else {
			++s;
		}
	}
}

void Fluid::Transmute() {
	SphereFluidMaterial* mat = dynamic_cast<SphereFluidMaterial*>(model->GetFragment("Main")->material);
	mat->positions.clear();
	for (DensityConstraint* constraint : densityConstraintList) {
		mat->positions.push_back(constraint->Center->x);
	}
}

json Fluid::GetJSON() {
	json j;
	return j;
}