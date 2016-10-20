#include "Fragment.h"
#include "../../VertexFormat.h"
using namespace Rendering;
using namespace GameObjects;
using namespace Models;
using namespace Materials;


Fragment::Fragment(const Fragment &other) {
	this->parent = other.parent;
	this->material = other.material;
	this->indexStartPointer = other.indexStartPointer;
	this->indexCount = other.indexCount;
	this->primitiveType = other.primitiveType;
}

Fragment::Fragment(Fragment &other, Model* parent) {
	this->parent = parent;
	this->material = other.material;
	this->indexStartPointer = other.indexStartPointer;
	this->indexCount = other.indexCount;
	this->primitiveType = other.primitiveType;
}

Fragment::Fragment(Model * parent, IMaterial * material, int startingIndex, GLuint indexCount, int startingVertex, int vertexCount, GLuint primitiveType)
{
	this->parent = parent;
	this->material = material;
	this->indexStartPointer = (GLvoid*)(sizeof(unsigned int) * startingIndex);
	this->indexCount = indexCount;
	this->startingVertex = startingVertex;
	this->vertexCount = vertexCount;
	this->primitiveType = primitiveType;
}

Fragment::~Fragment() {}