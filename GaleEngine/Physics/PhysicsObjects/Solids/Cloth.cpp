#include "Cloth.h"
#include "../../Constraints/StretchingConstraint.h"
#include "../../Constraints/FaceBendingConstraint.h"
#include "../../Particles/Particle.h"
#include "../../../Rendering/GameObjects/Models/Model.h"
#include "Triangle.h"
#include <unordered_map>

//REMOVE
#include <glm/glm.hpp>

using namespace Physics;
using namespace PhysicsObjects;
using namespace Constraints;
using namespace Particles;
using namespace Solids;
using namespace Rendering::GameObjects::Models;
using namespace std;
using json = nlohmann::json;

Cloth::Cloth() : PhysicsObject() { }

Cloth::~Cloth() {
	for (StretchingConstraint* s : stretchingConstraintList) {
		delete s;
	}

	for (FaceBendingConstraint* f : faceBendingConstraintList) {
		delete f;
	}
}

Cloth::Cloth(Model* model, unsigned int indexStart, unsigned int numTriangles, float stretchingStiffness, float bendingStiffness) : PhysicsObject() {
	this->model = model;
	this->indexStart = indexStart;
	this->numTriangles = numTriangles;
	vector<Triangle*> triangleList;

	unsigned int indexEnd = indexStart + 3 * numTriangles;

	for (unsigned int i = indexStart; i < indexEnd; i++) {
		int vertInd = model->GetIndex(i);
		if (particleList[vertInd] == nullptr) {
			particleList[vertInd] = new Particle(model->GetVertWorldPos(vertInd));
		}
	}

	Particle* p1, *p2, *p3, *p4;
	StretchingConstraint* c1, *c2, *c3;
	for (unsigned int i = indexStart; i < indexEnd; i += 3) {
		p1 = p2 = p3 = p4 = nullptr;
		c1 = c2 = c3 = nullptr;

		p1 = particleList[model->GetIndex(i)];
		p2 = particleList[model->GetIndex(i + 1)];
		p3 = particleList[model->GetIndex(i + 2)];

		for (StretchingConstraint* constraint : stretchingConstraintList) {
			if (constraint->ContainsParticles(p1, p2)) {
				c3 = constraint;
			}
			else if (constraint->ContainsParticles(p2, p3)) {
				c1 = constraint;
			}
			else if (constraint->ContainsParticles(p3, p1)) {
				c2 = constraint;
			}
		}

		if (c1 == nullptr) {
			stretchingConstraintList.push_back(new StretchingConstraint(p2, p3, stretchingStiffness));
		}
		else {
			for (Triangle* tri : triangleList) {
				p4 = tri->GetOther(p2, p3);
				if (p4 != nullptr) {
					faceBendingConstraintList.push_back(new FaceBendingConstraint(p2, p3, p1, p4, bendingStiffness));
				}
			}
		}
		if (c2 == nullptr) {
			stretchingConstraintList.push_back(new StretchingConstraint(p3, p1, stretchingStiffness));
		}
		else {
			for (Triangle* tri : triangleList) {
				p4 = tri->GetOther(p3, p1);
				if (p4 != nullptr) {
					faceBendingConstraintList.push_back(new FaceBendingConstraint(p3, p1, p2, p4, bendingStiffness));
				}
			}
		}
		if (c3 == nullptr) {
			stretchingConstraintList.push_back(new StretchingConstraint(p1, p2, stretchingStiffness));
		}
		else {
			for (Triangle* tri : triangleList) {
				p4 = tri->GetOther(p1, p2);
				if (p4 != nullptr) {
					faceBendingConstraintList.push_back(new FaceBendingConstraint(p1, p2, p3, p4, bendingStiffness));
				}
			}
		}

		triangleList.push_back(new Triangle(p1, p2, p3));
	}
}

void Cloth::Project(int iterations) {
	for (StretchingConstraint* constraint : stretchingConstraintList) {
		constraint->UpdateDerivs();
		float iter_stiffness = 1.0f - pow(1.0f - constraint->stiffness, 1.0f / (float)iterations);
		for (int i = 0; i < 2; i++) {
			constraint->ParticleList[i]->p += -iter_stiffness * constraint->s * constraint->ParticleList[i]->w * constraint->ParticleGradients[i];
		}
	}
	for (FaceBendingConstraint* constraint : faceBendingConstraintList) {
		constraint->UpdateDerivs();
		float iter_stiffness = 1.0f - pow(1.0f - constraint->stiffness, 1.0f / (float)iterations);
		for (int i = 0; i < 4; i++) {
			constraint->ParticleList[i]->p += -iter_stiffness * constraint->s * constraint->ParticleList[i]->w * constraint->ParticleGradients[i];
		}
	}
}

void Cloth::DeleteParticle(Particle* particle) {
	vector<StretchingConstraint*>::iterator s = stretchingConstraintList.begin();
	while (s != stretchingConstraintList.end()) {
		if ((*s)->ContainsParticle(particle)) {
			delete (*s);
			s = stretchingConstraintList.erase(s);
		}
		else {
			++s;
		}
	}
	vector<FaceBendingConstraint*>::iterator f = faceBendingConstraintList.begin();
	while (f != faceBendingConstraintList.end()) {
		if ((*f)->ContainsParticle(particle)) {
			delete (*f);
			f = faceBendingConstraintList.erase(f);
		}
		else {
			++f;
		}
	}
}

void Cloth::Transmute() {
	for (auto kv : particleList) {
		model->UpdatePosition(kv.first, kv.second->x);
	}
	model->RecalculateNormals();
	model->InvalidateVBO();
}

vector<string> Cloth::GetDebugOutput() {
	return { "Cloth: Nothing to display." };
}

void Cloth::FixParticle(unsigned int particleIndex) {
	particleList[particleIndex]->Fix();
}

json Cloth::GetJSON() {
	json j;
	j["Type"] = "Cloth";
	j["Model"] = model->name;
	j["IndexStart"] = indexStart;
	j["NumTriangles"] = numTriangles;
	return j;
}