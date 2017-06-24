#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Rendering {
	namespace UI {
		class UIModel {
		public:
			const UIModel* parent;
			std::string name;
			std::vector<UIModel*> children;
			glm::mat3 toParentMatrix;
			glm::mat3 toWorldMatrix;
			glm::mat3 toWorldMatrixInv;

			UIModel(std::string name);
			virtual ~UIModel() = 0;
			virtual void Destroy();

			virtual void AddMeToChildren(UIModel* child);
			virtual void UpdateParent(UIModel* parent);
			virtual void DeleteFromChildren(UIModel* )

		};
	}
}