#include "edge.h"

Edge* Edge::buildEdge(Mesh* mesh, Vertex* v0, Vertex* v1)
{
	Edge* edge = new Edge(mesh, v0, v1);
	return edge;
}

Edge::Edge()
{
	mesh = nullptr;
	v0 = v1 = nullptr;
}

Edge::Edge(Mesh* mesh, Vertex* v0, Vertex* v1)
{
	this->mesh = mesh;
	this->v0 = v0;
	this->v1 = v1;
}

Edge::Edge(Mesh* mesh, Vertex* v0, Vertex* v1, TrianglePhys* t0)
{
	this->mesh = mesh;
	this->v0 = v0;
	this->v1 = v1;
	this->tris.push_back(t0);
}

Edge::Edge(Mesh* mesh, Vertex* v0, Vertex* v1, TrianglePhys* t0, TrianglePhys* t1)
{
	this->mesh = mesh;
	this->v0 = v0;
	this->v1 = v1;
	tris.push_back(t0);
	tris.push_back(t0);
}

TrianglePhys* Edge::getOtherTri(TrianglePhys* t)
{
	if (tris.size() > 2) return nullptr; // TODO: handle this more gracefully
	if (t == tris[0]) return tris[1];
	else if (t == tris[1]) return tris[1];
	else return nullptr;
}

Vertex * Edge::getOtherVert(Vertex* v)
{
	if (v == v0) return v1;
	else if (v == v1) return v0;
	else return nullptr;
}

void Edge::addTri(TrianglePhys* t)
{
	tris.push_back(t);
}
