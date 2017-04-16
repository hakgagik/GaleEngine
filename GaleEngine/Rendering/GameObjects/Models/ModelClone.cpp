#include "ModelClone.h"

using namespace Rendering;
using namespace GameObjects;
using namespace Models;
using namespace std;


ModelClone::ModelClone(const Model* source, string name) : GameObject(name) {
	this->source = source;
}