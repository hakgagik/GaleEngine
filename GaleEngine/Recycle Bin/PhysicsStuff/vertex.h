#pragma once
#include <vector>
#include <glm\glm.hpp>

class TrianglePhys;
class Edge;
class Mesh;

class Vertex {
public:
	std::vector<TrianglePhys*> tris;
	std::vector<Edge*> edges;
	glm::vec3 pos;

	Mesh* mesh;

	Vertex();
	Vertex(Mesh* mesh, glm::vec3 pos);
};