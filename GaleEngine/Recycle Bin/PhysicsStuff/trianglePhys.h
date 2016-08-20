#pragma once

class Mesh;
class Vertex;
class Edge;

class TrianglePhys {
public:
	Mesh* mesh;
	Vertex** verts;
	Edge** edges;

	TrianglePhys();
	TrianglePhys(Mesh* mesh, Vertex* v0, Vertex* v1, Vertex* v2);
	TrianglePhys(Mesh* mesh, Vertex* v0, Vertex* v1, Vertex* v2, Edge* e0, Edge* e1, Edge* e2);
};