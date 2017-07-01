#pragma once
#include <string>
#include <vector>

namespace Rendering {
	class ISceneNode {
	public:
		const ISceneNode* parent;
		std::string name;
		std::vector<ISceneNode*> children;

		virtual void AddMeToChildren(ISceneNode* child);
		virtual void UpdateParent(ISceneNode* parent);
		virtual void DeleteFromChildren(ISceneNode* child);
	};
}