#pragma once
#include<vector>

class Mesh;
class Vertex;
class Edge;
class TrianglePhys;

class PhysObj {
public:
	Mesh* mesh;
	std::vector<Vertex*> verts;
	std::vector<Edge*> edges;
	std::vector<TrianglePhys*> tris;

	PhysObj();
	PhysObj(Mesh* mesh);
};