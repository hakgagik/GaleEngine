#pragma once
#include <glm/glm.hpp>

namespace Physics {
	struct Collision {
	public:
		float t;
		glm::vec3 collisionPoint;
		Collision() {
			this->t = HUGE_VALF;
			this->collisionPoint = glm::vec3(HUGE_VALF, HUGE_VALF, HUGE_VALF);
		}

		Collision(float t, glm::vec3 &collisionPoint) {
			this->t = t;
			this->collisionPoint = collisionPoint;
		}
	};
}