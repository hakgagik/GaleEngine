#include "Fluid.h"
#include "FluidHelper.h"
#include "../../Particles/Particle.h"
#include "../../Constraints/DensityConstraint.h"
#include "../../../Managers/Physics_Manager.h"
#include "../../../Rendering/GameObjects/Models/Model.h"
#include "../../../Rendering/GameObjects/Models/Fragment.h"
#include "../../../Rendering/Materials/SphereFluidMaterial.h"
#include <unordered_map>
#include <sstream>
#include <iomanip>

using namespace Physics;
using namespace Particles;
using namespace Constraints;
using namespace PhysicsObjects::Fluids;
using namespace Rendering::GameObjects::Models;
using namespace Rendering::Materials;
using namespace Managers;
using namespace std;
using namespace glm;
using json = nlohmann::json;

Fluid::Fluid() : PhysicsObject() {}

Fluid::Fluid(Model* model, vector<vec3> &positions, float particleMass, float restDensity, float densityStiffness)
{
	this->model = model;
	this->restDensity = restDensity;
	this->densityStiffness = densityStiffness;
	
	int tick = 0;
	for (vec3 pos : positions) {
		Particle* p = new Particle(pos, vec3(0), particleMass);
		DensityConstraint* c = new DensityConstraint(p, this->restDensity, this->densityStiffness);
		densityConstraintList.push_back(c);
		particleList[tick] = c->Center;
		tick++;
	}

	if (restDensity < 0) {
		FluidHelper tempFluidHelper = FluidHelper::GetThrowaway();
		tempFluidHelper.AddParticles(densityConstraintList);

		tempFluidHelper.Update();
		unordered_map<float, int> densityCounts;
		for (DensityConstraint* c : densityConstraintList) {
			c->FindNeighbors(tempFluidHelper);
			densityCounts[c->CalculateLocalDensity()]++;
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
		//this->restDensity = 6378.0f;

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

void Fluid::CalculatePotentialInteractions() {
	//for (DensityConstraint* constraint : densityConstraintList) {
#pragma omp parallel for
	for (int i = 0; i < densityConstraintList.size(); i++) {
		densityConstraintList[i]->FindNeighbors(FluidHelper::Get());
	}
}

void Fluid::Project(int iterations) {

	for (DensityConstraint* constraint : densityConstraintList)  {
		constraint->UpdateDerivs();
	}

	for (DensityConstraint* constraint : densityConstraintList) {
		//float iter_stiffness = 1.0f - pow(1.0f - constraint->stiffness, 1.0f / (float)iterations);
		constraint->Center->dp = /*iter_stiffness **/ constraint->GetDP();
	}
	for (DensityConstraint* constraint : densityConstraintList) {
		constraint->Center->p += constraint->Center->dp;
	}
}

void Fluid::FinalizeParticles() {
	float dt = Physics_Manager::Get().dt;
	for (auto kv : particleList) {
		kv.second->v = (kv.second->p - kv.second->x) / dt;
	}

	for (DensityConstraint* constraint : densityConstraintList) {
		constraint->Center->v += constraint->GetFVC() * constraint->Center->w * dt;
	}

	//TODO: this looks like it's time step dependant. Make sure it's not

	for (DensityConstraint* constraint : densityConstraintList) {
		vec3 dv = constraint->GetDP();
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

vector<string> Fluid::GetDebugOutput() {
	vector<string> output(3);
	stringstream ss;
	ss << fixed << setprecision(2);

	ss << "Fluid " << model->name << ":";
	output[0] = ss.str();

	ss.str("");
	ss << "Rho_0: " << restDensity;
	output[1] = ss.str();

	int totalGradients = 0;
	//for (DensityConstraint* constraint : densityConstraintList) {
	//	totalGradients += constraint->ParticleGradients.size();
	//}
	
	ss.str("");
	ss << "Num gradients: " << totalGradients;
	output[2] = ss.str();

	return output;
}

json Fluid::GetJSON() {
	json j;
	return j;
}