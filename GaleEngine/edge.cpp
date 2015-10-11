#include "edge.h"

Edge::Edge()
{
	mesh = nullptr;
	v0 = v1 = nullptr;
	t0 = t1 = nullptr;
}

Edge::Edge(Mesh* mesh, Vertex* v0, Vertex* v1)
{
	this->mesh = mesh;
	this->v0 = v0;
	this->v1 = v1;
}

Edge::Edge(Mesh* mesh, Vertex* v0, Vertex* v1, Triangle* t0)
{
	this->mesh = mesh;
	this->v0 = v0;
	this->v1 = v1;
	this->t0 = t0;
}

Edge::Edge(Mesh* mesh, Vertex* v0, Vertex* v1, Triangle* t0, Triangle* t1)
{
	this->mesh = mesh;
	this->v0 = v0;
	this->v1 = v1;
	this->t0 = t0;
	this->t1 = t1;
}

Triangle* Edge::getOtherTri(Triangle* t)
{
	if (t == t0) return t1;
	else if (t == t1) return t0;
	else return nullptr;
}

Vertex * Edge::getOtherVert(Vertex* v)
{
	if (v == v0) return v1;
	else if (v == v1) return v0;
	else return nullptr;
}

void Edge::addTri(Triangle* t)
{
	if (t0 == nullptr) t0 = t;
	else if (t1 == nullptr) t1 = t;
}
