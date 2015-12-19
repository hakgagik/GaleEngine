#pragma once
#include "lib/glew/glew.h"
#include "lib/freeglut/freeglut.h"

namespace Core {
	struct FramebufferInfo {
		unsigned int flags;
		bool msaa;

		FramebufferInfo() {
			flags = GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH;
			msaa = false;
		}

		FramebufferInfo(bool color, bool depth, bool stencil, bool msaa) {
			flags = GLUT_DOUBLE;
			if (color) flags |= GLUT_RGBA;
			if (depth) flags |= GLUT_DEPTH;
			if (stencil) flags |= GLUT_STENCIL;
			if (msaa) flags |= GLUT_MULTISAMPLE;
			this->msaa = msaa;
		}

		FramebufferInfo(FramebufferInfo& framebufferInfo) {
			this->flags = flags;
			this->msaa = msaa;
		}

		void operator=(FramebufferInfo& framebufferInfo) {
			this->flags = flags;
			this->msaa = msaa;
		}
	};
}