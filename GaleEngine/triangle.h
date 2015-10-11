#pragma once

class Mesh;
class Vertex;
class Edge;

class Triangle {
public:
	Mesh* mesh;
	Vertex** verts;
	Edge** edges;

	Triangle();
	Triangle(Mesh* mesh, Vertex* v0, Vertex* v1, Vertex* v2);
	Triangle(Mesh* mesh, Vertex* v0, Vertex* v1, Vertex* v2, Edge* e0, Edge* e1, Edge* e2);
};