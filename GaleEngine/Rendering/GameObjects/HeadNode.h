#pragma once
#include "IGameObject.h"

namespace Rendering {
	namespace GameObjects {
		class HeadNode : public IGameObject {
		public:
			virtual ~HeadNode() override;

			virtual void updateParent(IGameObject* parent) override;
			virtual void addToSceneTree(IGameObject* parent, std::string name, glm::vec3 position, glm::quat orientation, glm::vec3 scale, bool enabled) override;
			virtual nlohmann::json GetJSON() override;

		protected:
			virtual void updateLocalMatrices() override;
		};
	}
}