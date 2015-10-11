#pragma once

#include<vector>

class Mesh;
class Vertex;
class Edge;
class Triangle;

class PhysObj {
public:
	Mesh* mesh;
	std::vector<Vertex*> verts;
	std::vector<Edge*> edges;
	std::vector<Triangle*> tris;
};