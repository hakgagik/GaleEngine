#pragma once
#include <vector>
#include <glm\glm.hpp>

class Mesh {
public:
	static Mesh* BuildMesh(const char* filename);

	std::string done;

	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> norms;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<unsigned int> uvIndices;

	Mesh();
};