// Modified from http://in2gpu.com/opengl-3/
#pragma once
#include <iostream>
#include "../ContextInfo.h"
#include "../FrameBufferInfo.h"
#include "../WindowInfo.h"
#include "../Init/Init_GLEW.h"
#include "../IListener.h"
#include "../DebugOutput.h"

namespace Core {
	namespace Init {
		class Init_GLUT {
		public:
			static void init(const Core::WindowInfo& window, const Core::ContextInfo& context, const Core::FramebufferInfo& framebufferInfo);
			static void run();
			static void close();
			static void enterFullscreen();
			static void exitFullscreen();
			static void printOpenGLInfo(const Core::WindowInfo& window, const Core::ContextInfo& context);
			static void setListener(IListener*& iListener);

		private:
			static IListener* listener;
			static WindowInfo windowInfo;

			static void idleCallback(void);
			static void displayCallback(void);
			static void reshapeCallback(int width, int height);
			static void closeCallback();
		};
	}
}