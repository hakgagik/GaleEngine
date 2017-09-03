// Modified from http://in2gpu.com/opengl-3/
#pragma once

namespace Core {
	// Struct to store info about the current OpenGL context version
	struct ContextInfo {
		// GL major version. Ingored if core is false.
		int major_version;
		// GL minor version. Ignored if core is false.
		int minor_version;
		// Is this a core version? Or are there extensions?
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