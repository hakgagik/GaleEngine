#pragma once
#include "../GameObject.h"

namespace Rendering {
	namespace GameObjects {
		namespace Models {
			class Model;

			class ModelClone : public GameObject {
			public:
				const Model* source;
				ModelClone(const Model* source, std::string name);
			};
		}
	}
}