#include "mesh.h"
#include <vector>
#include <glm\glm.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

void splitFaceVert(std::string vstring, int& vi, int& ni, int& uvi) {

	int slash1 = int(vstring.find('/')), slash2 = int(vstring.find('/', slash1 + 1)), numSlashes = 2;
	if (slash1 == std::string::npos) numSlashes = 0;
	else if (slash2 == std::string::npos) numSlashes = 1;
	vi = ni = uvi = -1;

	//for (int i = 0; i < vstring.size(); i++) {
	//	if (vstring[i] == '/') {
	//		switch (numSlashes) {
	//		case 0:
	//			slash1 = i;
	//			numSlashes++;
	//			break;
	//		case 1:
	//			slash2 = i;
	//			numSlashes++;
	//			break;
	//		}
	//	}
	//	if (numSlashes > 1) break;
	//}

	switch (numSlashes) {
	case 0:
		vi = atoi(vstring.c_str()) - 1;
		break;
	case 1:
		vi = atoi(vstring.substr(0, slash1).c_str()) - 1;
		ni = atoi(vstring.substr(slash1 + 1).c_str()) - 1;
		break;
	case 2:
		vi = atoi(vstring.substr(0, slash1).c_str()) - 1;
		ni = atoi(vstring.substr(slash1 + 1, slash2 - slash1).c_str()) - 1;
		uvi = atoi(vstring.substr(slash2 + 1).c_str()) - 1;
		break;
	}
}

Mesh* Mesh::BuildMesh(const char* filename)
{
	Mesh* mesh = new Mesh();
	//FILE* file = fopen(filename, "r");
	//if (file == NULL) {
	//	printf("Couldn't open obj file.\n");
	//}

	std::ifstream infile(filename);

	std::string line, bit, vstring0, vstring;

	int vi, ni, uvi, vi0, ni0, uvi0;

	while (std::getline(infile, line)) {
		std::istringstream iss(line);
		iss >> bit;
		if (bit == "v") {
			glm::vec3 vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			mesh->verts.push_back(vertex);
		}
		else if (bit == "vn") {
			glm::vec3 normal;
			iss >> normal.x >> normal.y >> normal.z;
			mesh->norms.push_back(normal);
		}
		else if (bit == "vt") {
			glm::vec2 uv;
			iss >> uv.x >> uv.y;
			mesh->uvs.push_back(uv);
		}
		else if (bit == "f") {
			std::vector<std::string> verticesInFace;
			while (iss >> bit) {
				verticesInFace.push_back(bit);

			}
			int numVertices = int(verticesInFace.size());
			if (numVertices < 3) throw 0; // TODO: throw an error here and exit

			// TODO: check to make sure all indices are >0. if not, handle appropriately
			vstring0 = verticesInFace[0];
			verticesInFace.erase(verticesInFace.begin());
			splitFaceVert(vstring0, vi0, ni0, uvi0);

			for (int i = 0; i < numVertices - 2; i++) {
				if (vi0 > -1) mesh->vertexIndices.push_back(vi0);
				if (ni0 > -1) mesh->normalIndices.push_back(ni0);
				if (uvi0 > -1) mesh->uvIndices.push_back(uvi0);
				for (int j = 0; j < 2; j++) {
					vstring = verticesInFace[0];
					verticesInFace.erase(verticesInFace.begin());
					splitFaceVert(vstring, vi, ni, uvi);
					if (vi > -1) mesh->vertexIndices.push_back(vi);
					if (ni > -1) mesh->normalIndices.push_back(ni);
					if (uvi > -1) mesh->uvIndices.push_back(uvi);
				}
				verticesInFace.push_back(vstring);
			}
		}
	}

	if (mesh->vertexIndices.size() == 0) throw 0; // Malformed obj fil
	if ((mesh->normalIndices.size() != 0 && mesh->normalIndices.size() != mesh->vertexIndices.size()) && (mesh->uvIndices.size() != 0 && mesh->uvIndices.size() != mesh->vertexIndices.size())) throw 0; // Malformed obj file
	return mesh;
}

Mesh::Mesh()
{
}
