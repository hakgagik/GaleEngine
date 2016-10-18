#pragma once

namespace Physics {
	namespace Particles {
		class Particle;
	}
	class IConstraintGroup {
	public:
		virtual void GenerateConstraints(float dt) = 0;
		virtual void Project(int iterations) = 0;
		virtual void DeleteParticle(Particles::Particle* particle) = 0;
	};
}