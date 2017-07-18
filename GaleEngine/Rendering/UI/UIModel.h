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
			virtual void DeleteFromChildren(UIModel* child);
			virtual void AddToSceneTree(UIModel* parent, glm::vec2 &position, float &orientation, glm::vec2 &scale, bool enabled = true);
			virtual void UpdateMatrices();
			virtual void InvalidateMatrices();

			bool enabled;

			virtual glm::vec2 getPosition() const;
			virtual float getOrientation() const;
			virtual glm::vec2 getScale() const;
			virtual int getDepth() const;
			
			virtual void setPosition(glm::vec2 &pos);
			virtual void setOrientation(float &orientation);
			virtual void setScale(glm::vec2 &scale);

		protected:
			glm::vec2 position; // relative to parent
			float orientation; // relative to parent
			glm::vec2 scale; // relative to parent
			int depth;

			bool matricesValid;
			virtual void updateLocalMatrices();
		};
	}
}