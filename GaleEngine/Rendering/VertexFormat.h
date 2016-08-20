#pragma once
#include <glm/glm.hpp>

namespace Rendering {
	struct VertexFormat {
		glm::vec4 position;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec4 tangent;

		VertexFormat(const glm::vec4 &iPos, const glm::vec3 &iNormal, const glm::vec2 &iUv, const glm::vec4 &iTangent) {
			position = iPos;
			normal = iNormal;
			uv = iUv;
			tangent = iTangent;
		}

		VertexFormat() {}
	};
}