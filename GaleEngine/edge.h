#pragma once
#include <vector>

class Mesh;
class Triangle;
class Vertex;

class Edge {
public:
	Mesh* mesh;
	Vertex* v0;
	Vertex* v1;
	Triangle* t0;
	Triangle* t1;

	Edge();
	Edge(Mesh* mesh, Vertex* v0, Vertex* v1);
	Edge(Mesh* mesh, Vertex* v0, Vertex* v1, Triangle* t0);
	Edge(Mesh* mesh, Vertex* v0, Vertex* v1, Triangle* t0, Triangle* t1);

	Triangle* getOtherTri(Triangle* t);
	Vertex* getOtherVert(Vertex* v);
	void addTri(Triangle* t);
};