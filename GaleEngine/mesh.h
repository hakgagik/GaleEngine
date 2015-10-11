#pragma once
#include <vector>
#include <glm\glm.hpp>

class Vertex;
class Triangle;
class Edge;

class Mesh {
public:
	static Mesh* BuildMesh(const char* filename);

	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned short> elements;
	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<unsigned int> uvIndices;

	std::vector<Vertex> vertices;
	std::vector<Edge> edges;
	std::vector<Triangle> tris;

	Mesh();
};