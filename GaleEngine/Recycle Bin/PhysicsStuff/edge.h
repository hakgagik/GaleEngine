#pragma once
#include <vector>

class Mesh;
class TrianglePhys;
class Vertex;

class Edge {
public:
	Mesh* mesh;
	Vertex* v0;
	Vertex* v1;
	std::vector<TrianglePhys*> tris;

	static Edge* buildEdge(Mesh* mesh, Vertex* v0, Vertex* v1);

	Edge();
	Edge(Mesh* mesh, Vertex* v0, Vertex* v1);
	Edge(Mesh* mesh, Vertex* v0, Vertex* v1, TrianglePhys* t0);
	Edge(Mesh* mesh, Vertex* v0, Vertex* v1, TrianglePhys* t0, TrianglePhys* t1);

	TrianglePhys* getOtherTri(TrianglePhys* t);
	Vertex* getOtherVert(Vertex* v);
	void addTri(TrianglePhys* t);
};