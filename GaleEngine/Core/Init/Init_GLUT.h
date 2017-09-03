// Modified from http://in2gpu.com/opengl-3/
#pragma once

namespace Core {
	struct WindowInfo;
	struct ContextInfo;
	struct FramebufferInfo;
	class ISceneListener;

	namespace Init {
		// The class used to initalize freeGLUT and setup the main control loop
		class Init_GLUT {
		public:
			// General function to Initialize GLUT
			// GLUT will handle generating the OpenGL context, as well as deciding what version of GL we will be using
			// Also in charge of calling glewInit
			static void Init(const WindowInfo& window, const ContextInfo& context, const FramebufferInfo& framebufferInfo);

			// Begin the main render loop. Notifies listener to intialize, the calls glutMainLoop.
			static void Run();
			
			// Notify listener of scene closing, and exit the glut loop.
			static void Close();

			// Print info about the OpenGL context that freeGLUT created
			static void PrintOpenGLInfo(const WindowInfo& window, const ContextInfo& context);

			// Set the active screen listener that will recieve window-related and rendering-related events from the OpenGL context
			static void SetListener(ISceneListener* iSceneListener);

		private:
			// The current scene listener that will receive messages
			static ISceneListener* listener;
			// Stores info about the window we're rendering to
			static WindowInfo windowInfo;

#pragma region callbacks
			// Callback called by glutIdleFunc. Called whenever no other GLUT events are occuring.
			// Fires another frame whenever there's nothing else to do.
			static void idleCallback(void);

			// Callback called by glutDisplayFunc. Called whenever glutPostRedisplay is called.
			// Sends sendering instructions to listener. 
			static void displayCallback(void);
			
			// Callback called by glutReshapeFunc. Called whenever the wondow size changes.
			// Sends info about new window size to listener.
			static void reshapeCallback(int width, int height);

			// Callback called by glutCloseFunc. Called when the window is closed.
			// Exits glutMainLoop.
			static void closeCallback();
#pragma endregion
		};
	}
}