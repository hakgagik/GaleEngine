#pragma once
#include "GameObject.h"

namespace Rendering {
	namespace GameObjects {
		class HeadNode : public GameObject {
		public:
			HeadNode(std::string name);
			virtual ~HeadNode() override;

			virtual void UpdateParent(GameObject* parent) override;
			virtual void AddToSceneTree(GameObject* parent, glm::vec3 &position, glm::quat &orientation, glm::vec3 &scale, bool enabled) override;
			virtual nlohmann::json GetSourceJSON() const override;

		protected:
			virtual void updateLocalMatrices() override;
		};
	}
}