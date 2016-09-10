#pragma once
#include "../IGameObject.h"

namespace Rendering {
	namespace GameObjects {
		namespace Models {
			class Model;

			class ModelClone : public IGameObject {
			public:
				const Model* source;
				ModelClone(const Model* source, std::string name);
			};
		}
	}
}