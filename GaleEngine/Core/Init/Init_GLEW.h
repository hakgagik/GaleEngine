#pragma once
// Modified from http://in2gpu.com/opengl-3/

namespace Core {
	namespace Init {
		class Init_GLEW {
		public:
			// Initialize GL Extension Wrangler
			// This allows for platform-independent calls to OpenGL extensions
			// The alternative (as far as I understand) is to include the appropriate headers from GL/* and include the appropriate (platform-specific) libraries in the project.
			static void Init();
		};
	}
}