#include "mesh.h"
#include <vector>
#include <glm\glm.hpp>
#include <fstream>
#include <sstream>

Mesh* Mesh::BuildMesh(const char* filename)
{
	Mesh* mesh = new Mesh();
	//FILE* file = fopen(filename, "r");
	//if (file == NULL) {
	//	printf("Couldn't open obj file.\n");
	//}

	std::ifstream infile(filename);

	std::vector<glm::vec3> temp_vertices, temp_normals;
	std::vector<glm::vec2> temp_uv;

	std::string line;
	std::string bit;

	while (std::getline(infile, line)) {
		std::istringstream iss(line);
		iss >> bit;
			if (bit == "v") {
				glm::vec3 vertex;
				iss >> vertex.x >> vertex.y >> vertex.z;
				temp_vertices.push_back(vertex);
			}
			else if (bit == "vn") {
				glm::vec3 normal;
				iss >> normal.x >> normal.y >> normal.z;
				temp_normals.push_back(normal);
			}
			else if (bit == "vt") {
				glm::vec2 uv;
				iss >> uv.x >> uv.y;
				temp_uv.push_back(uv);
			}
			else if (bit == "f") {
				std::vector<std::string> vertices;
				while (iss) {
					iss >> bit;
					vertices.push_back(bit);

				}
				int numVertices = vertices.size();

				for (int i = 0; i < numVertices; i++) {

				}

			}
	}

	//while (true) {
	//	char lineHeader[256];
	//	int res = fscanf(file, "%s", lineHeader);
	//	if (res == EOF) break;
	//	if (strcmp(lineHeader, "v") == 0) {
	//		glm::vec3 vertex;
	//		fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
	//		temp_vertices.push_back(vertex);
	//	}
	//	else if (strcmp(lineHeader, "vn") == 0) {
	//		glm::vec3 normal;
	//		fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
	//		temp_normals.push_back(normal);
	//	}
	//	else if (strcmp(lineHeader, "vt") == 0) {
	//		glm::vec2 uv;
	//		fscanf(file, "%f %f\n", &uv.x, &uv.y);
	//		temp_uv.push_back(uv);
	//	}
	//	else if (strcmp(lineHeader, "f") == 0) {
	//		std::string vertex1, vertex2, vertex3;
	//		unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	//		//-------------------CONTINUE HERE--------------------//
	//		int matches = fscanf(file, "FILL THIS IN"); // TODO: CONTINUE HERE
	//	}
	//}

	return mesh;
}

Mesh::Mesh()
{
}
