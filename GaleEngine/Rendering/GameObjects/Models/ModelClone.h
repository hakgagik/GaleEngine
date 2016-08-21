#pragma once
#include "../IGameObject.h"

namespace Rendering {
	namespace GameObjects {
		namespace Models {
			class Model;

			class ModelClone : public IGameObject {
			public:
				Model* source;
				ModelClone(Model* source, std::string name);
			};
		}
	}
}