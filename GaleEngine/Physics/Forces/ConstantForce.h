#pragma once
#include "Force.h"

namespace Physics {
	namespace Forces {
		class ConstantForce : public Force {
		public:
			ConstantForce(glm::vec3 &acceleration);
			virtual glm::vec3 GetForce(Particles::Particle* particle) override;
		protected:
			glm::vec3 acc;
		};
	}
}