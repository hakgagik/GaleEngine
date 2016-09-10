#pragma once
#include "IGameObject.h"

namespace Rendering {
	namespace GameObjects {
		class HeadNode : public IGameObject {
		public:
			HeadNode(std::string name);
			virtual ~HeadNode() override;

			virtual void UpdateParent(IGameObject* parent) override;
			virtual void AddToSceneTree(IGameObject* parent, glm::vec3 &position, glm::quat &orientation, glm::vec3 &scale, bool enabled) override;
			virtual nlohmann::json GetSourceJSON() const override;

		protected:
			virtual void updateLocalMatrices() override;
		};
	}
}