#pragma once
#include <string>

namespace Managers {
	class UI_Manager {
	public:
		static void Init();
		static UI_Manager& Get();

	private:
		UI_Manager();
		static UI_Manager instance;
	};
}