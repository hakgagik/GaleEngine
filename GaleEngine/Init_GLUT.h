#pragma once
#include "ContextInfo.h"
#include "FrameBufferInfo.h"
#include "WindowInfo.h"
#include <iostream>
#include "Init_GLEW.h"

namespace Core {
	namespace Init {
		class Init_GLUT {
		public:
			static void init(const Core::WindowInfo& window, const Core::ContextInfo& context, const Core::FramebufferInfo& framebufferInfo);
			static void run();
			static void close();
			void enterFullscreen();
			void exitFullscreen();
			static void printOpenGLInfo(const Core::WindowInfo& window, const Core::ContextInfo& context);

		private:
			static void idleCallback(void);
			static void displayCallback(void);
			static void reshapeCallback(int width, int height);
			static void closeCallback();
		};
	}
}