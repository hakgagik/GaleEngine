// Modified from http://in2gpu.com/opengl-3/
#pragma once

namespace Core {
	struct ContextInfo {
		int major_version, minor_version;
		bool core;

		ContextInfo() {
			major_version = 3;
			minor_version = 3;
			core = true;
		}

		ContextInfo(int major_version, int minor_version, bool core) {
			this->major_version = major_version;
			this->minor_version = minor_version;
			this->core = core;
		}

		ContextInfo(const ContextInfo& contextInfo) {
			major_version = contextInfo.major_version;
			minor_version = contextInfo.minor_version;
			this->core = core;
		}
		void operator=(const ContextInfo& contextInfo) {
			major_version = contextInfo.major_version;
			minor_version = contextInfo.minor_version;
			this->core = core;
		}
	};
}