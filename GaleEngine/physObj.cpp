#include "physObj.h"
#include "mesh.h"
#include "triangle.h"
#include "edge.h"
#include "vertex.h"
#include <glm\glm.hpp>
#include <string>
#include <unordered_map>

using namespace std;

PhysObj::PhysObj()
{
}

string edgeHash(int i0, int i1) {
	if (i1 > i0) return to_string(i0) + " " + to_string(i1);
	else return to_string(i1) + " " + to_string(i0);
}

PhysObj::PhysObj(Mesh * mesh)
{
	for (vector<glm::vec3>::iterator it = mesh->verts.begin(); it != mesh->verts.end(); ++it) {
		Vertex vert(mesh, *it);
		verts.push_back(&vert);
	}

	int triVerts = mesh->vertexIndices.size();

	unordered_map<string, Edge*> edgemap;
	int vi0, vi1, vi2;
	string key0, key1, key2;
	Edge *e0, *e1, *e2;
	for (int i = 0; i < triVerts; i += 3) {
		vi0 = mesh->vertexIndices[i];
		vi1 = mesh->vertexIndices[i + 1];
		vi2 = mesh->vertexIndices[i + 2];
		key0 = edgeHash(vi0, vi1);
		key1 = edgeHash(vi1, vi2);
		key2 = edgeHash(vi2, vi0);

		if (edgemap.count(key0) == 0) {
			e0 = Edge::buildEdge(mesh, verts[vi0], verts[vi1]);
			edgemap.emplace(key0, e0);
		}
		else {
			e0 = edgemap.at(key0);
		}

		if (edgemap.count(key1) == 0) {
			e1 = Edge::buildEdge(mesh, verts[vi1], verts[vi2]);
			edgemap.emplace(key1, e1);
		}
		else {
			e1 = edgemap.at(key1);
		}

		if (edgemap.count(key2) == 0) {
			e2 = Edge::buildEdge(mesh, verts[vi2], verts[vi0]);
			edgemap.emplace(key2, e2);
		}
		else {
			e2 = edgemap.at(key2);
		}

		
	}

}
