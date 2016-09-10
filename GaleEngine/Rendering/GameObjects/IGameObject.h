#pragma once
#include "../../lib/json.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>

namespace Rendering {
	namespace GameObjects {
		class IGameObject {
		public:
			const IGameObject* parent;
			std::string name;
			std::vector<IGameObject*> children;
			glm::mat4 toParentMatrix;
			glm::mat4 toWorldMatrix;

			IGameObject(std::string name);
			virtual ~IGameObject() = 0; // pure virtual destructor!
			virtual void Destroy();

			virtual void AddMeToChildren(IGameObject* child);
			virtual void UpdateParent(IGameObject* parent);
			virtual void DeleteFromChildren(IGameObject* child);
			virtual void AddToSceneTree(IGameObject* parent, glm::vec3 &position, glm::quat &orientation, glm::vec3 &scale, bool enabled = true);
			virtual void UpdateMatrices();
			virtual void InvalidateMatrices();

			bool enabled;

			virtual glm::vec3 getPosition() const;
			virtual glm::quat getOrientation() const;
			virtual glm::vec3 getScale() const;

			virtual void setPosition(glm::vec3 pos);
			virtual void setOrientation(glm::quat orientation);
			virtual void setScale(glm::vec3 scale);

			virtual nlohmann::json GetSourceJSON() const;
			nlohmann::json GetSceneJSON() const;
		protected:
			glm::vec3 position; //relative to parent
			glm::quat orientation; // relative to parent
			glm::vec3 scale; //relative to parent

			bool matricesValid;
			virtual void updateLocalMatrices();
		};
	}
}