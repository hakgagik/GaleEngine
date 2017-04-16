#pragma once
#include <string>
#include <vector>
#include <unordered_set>

namespace Core {
	class IDebuggable;
}

namespace Rendering {
	class IRenderer;
	namespace GameObjects {
		namespace Models {
			class Model;
		}
	}
}

namespace Managers {
	class UI_Manager {
	public:
		static UI_Manager& Get();

		void Init();
		void Draw(Rendering::IRenderer* renderer);
		void Resize(int width, int height);
		void EndFrame();
		void AddToDisplay(std::string &str);
		void RegisterDebuggable(Core::IDebuggable* debuggable);
		void DeregisterDebuggable(Core::IDebuggable* debuggable);

		int framerate;
	private:
		UI_Manager();
		static UI_Manager instance;
		Rendering::GameObjects::Models::Model* screenQuad;
		std::vector<std::string> toDisplay;
		std::unordered_set<Core::IDebuggable*> debuggableList;
		float x;
		float y;
		float sx;
		float sy;
		float fontScale;
		

		void printLine(const std::string& text, Rendering::IRenderer* renderer);
	};
}