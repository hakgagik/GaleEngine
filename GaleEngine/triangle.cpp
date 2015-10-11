#include "triangle.h"

Triangle::Triangle()
{
	mesh = nullptr;
	verts = new Vertex*[3];
	edges = new Edge*[3];
}

Triangle::Triangle(Mesh* mesh, Vertex* v0, Vertex* v1, Vertex* v2)
{
	this->mesh = mesh;
	verts[0] = v0;
	verts[1] = v1;
	verts[2] = v2;
}

Triangle::Triangle(Mesh* mesh, Vertex* v0, Vertex* v1, Vertex* v2, Edge* e0, Edge* e1, Edge* e2)
{
	this->mesh = mesh;
	verts[0] = v0;
	verts[1] = v1;
	verts[2] = v2;
	edges[0] = e0;
	edges[1] = e1;
	edges[2] = e2;
}
