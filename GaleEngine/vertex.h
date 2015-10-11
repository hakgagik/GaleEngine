#pragma once
#include <vector>
#include <glm\glm.hpp>

class Triangle;
class Edge;
class Mesh;

class Vertex {
public:
	std::vector<Triangle*> tris;
	std::vector<Edge*> edges;
	glm::vec3 pos;

	Mesh* mesh;

	Vertex();
	Vertex(Mesh* mesh, glm::vec3 pos);
};