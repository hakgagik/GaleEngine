#include "ModelClone.h"

using namespace Rendering;
using namespace GameObjects;
using namespace Models;
using namespace std;


ModelClone::ModelClone(Model* source, string name) : IGameObject(name) {
	this->source = source;
}