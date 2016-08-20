#pragma once
#include <GL/glew.h>

namespace Rendering {
	namespace Materials {
		struct IMaterial;
	}
	namespace GameObjects {
		namespace Models {
			class Model;
			class Fragment {
			public:
				// Type of primitive to draw this fragment with
				GLuint primitiveType = GL_TRIANGLES;
				// Pointer to starting position in paren't IBO. Should be starting index * sizeof(unsigned int)
				GLvoid* indexStartPointer = 0;
				// Number of vertices this fragment contains
				GLuint indexCount = 0;
				Rendering::Materials::IMaterial* material;
				// Model this fragment is part of
				Model* parent;

				int startingVertex;
				int vertexCount;

				Fragment(const Fragment &other);
				Fragment(Fragment &other, Model* parent);
				Fragment(Models::Model* parent, Rendering::Materials::IMaterial* material, int startingIndex, GLuint indexCount, int startingVertex, int vertexCount, GLuint primitiveType = GL_TRIANGLES);
				~Fragment();
			};
		}
	}
}