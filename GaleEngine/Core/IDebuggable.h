#pragma once
#include <vector>
#include <string>

namespace Core {
	class IDebuggable {
	public:
		virtual std::vector<std::string> GetDebugOutput() = 0;
	};
}