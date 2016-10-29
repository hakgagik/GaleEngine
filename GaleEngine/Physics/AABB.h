#pragma once
#include <glm/glm.hpp>

namespace Physics {
	struct Collision;

	class AABB {
	public:
		float xmin;
		float xmax;
		float ymin;
		float ymax;
		float zmin;
		float zmax;
		
		AABB(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);

		bool Collide(const AABB* other);
		Collision* Collide(const glm::vec3 &origin, const glm::vec3 &direction);
	};
}