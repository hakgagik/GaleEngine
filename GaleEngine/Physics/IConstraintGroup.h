#pragma once
#include "../Core/IDebuggable.h"

namespace Physics {
	namespace Particles {
		class Particle;
	}
	class IConstraintGroup : public Core::IDebuggable {
	public:
		virtual void GenerateConstraints(float dt) = 0;
		virtual void Project(int iterations) = 0;
		virtual void DeleteParticle(Particles::Particle* particle) = 0;
		virtual std::vector<std::string> GetDebugOutput() override = 0;
	};
}