// Modified from http://in2gpu.com/opengl-3/
#pragma once

namespace Core {
	struct WindowInfo;
	struct ContextInfo;
	struct FramebufferInfo;
	class ISceneListener;

	namespace Init {
		class Init_GLUT {
		public:
			static void init(const WindowInfo& window, const ContextInfo& context, const FramebufferInfo& framebufferInfo);
			static void run();
			static void close();
			static void enterFullscreen();
			static void exitFullscreen();
			static void printOpenGLInfo(const WindowInfo& window, const ContextInfo& context);
			static void setListener(ISceneListener* &iSceneListener);

		private:
			static ISceneListener* listener;
			static WindowInfo windowInfo;

			static void idleCallback(void);
			static void displayCallback(void);
			static void reshapeCallback(int width, int height);
			static void closeCallback();
		};
	}
}