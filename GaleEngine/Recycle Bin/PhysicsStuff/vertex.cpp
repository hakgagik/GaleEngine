#include "vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(Mesh * mesh, glm::vec3 pos)
{
	this->mesh = mesh;
	this->pos = pos;
}
