// Modified from http://in2gpu.com/opengl-3/
#include "Init_GLUT.h"
#include "Init_GLEW.h"
#include "../WindowInfo.h"
#include "../ContextInfo.h"
#include "../FrameBufferInfo.h"
#include "../ISceneListener.h"
#include "../DebugOutput.h"
#include "../../Rendering/IRenderer.h"
#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"

using namespace Core;
using namespace Init;
using namespace Rendering;
using namespace std;

ISceneListener* Init_GLUT::listener = NULL;
WindowInfo Init_GLUT::windowInfo;

// Perform GLUT-level setup
void Init_GLUT::Init(const WindowInfo & window, const ContextInfo & context, const FramebufferInfo & framebufferInfo)
{
	// store window info
	windowInfo = window;
	// create fake arguements to pass into glutInit()
	int fakeargc = 1;
	char *fakeargv[] = { "fake", NULL };
	glutInit(&fakeargc, fakeargv); // we don't need glut to process command line arguements

	if (context.core) {
		// Initialize the GL context
		glutInitContextVersion(context.major_version, context.minor_version);
		glutInitContextProfile(GLUT_CORE_PROFILE); // TODO: learn more about context profile
	}
	else {
		glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	}

	glutInitDisplayMode(framebufferInfo.flags); // Set display mode (GLUT DOUBLE, RGB, DEPTH, etc)
	glutInitWindowPosition(window.position_x, window.position_y); // set window position
	glutInitWindowSize(window.width, window.height); // set window size
	glutInitContextFlags(GLUT_DEBUG); // enable debugging
	glutCreateWindow(window.name.c_str()); // finally create the window

	cout << "GLUT: Initialized" << endl;

	glEnable(GL_DEBUG_OUTPUT);
	// now intialize glew
	Init_GLEW::Init();
	glDebugMessageCallback((GLDEBUGPROC)DebugOutput::DebugMessageCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	// set the callback functions
	glutIdleFunc(idleCallback);
	glutCloseFunc(closeCallback);
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);
	

	// cleanup?
	// when the window is closed, glut main loop returns
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// print out info on the window we just created
	PrintOpenGLInfo(window, context);
}

void Init_GLUT::Run() {
	cout << "GLUT:\tStart Running " << endl;
	listener->NotifySceneInit();
	glutMainLoop();

	//if (listener != nullptr && listener->sceneInitialized) {
	//	cout << "GLUT:\tStart Running " << endl;
	//	glutMainLoop();
	//}
	//else {
	//	cout << "Init_GLUT:\tScene not initalized, cannot start." << endl;
	//}
}

void Init_GLUT::Close() {
	cout << "GLUT:\tFinished" << endl;
	glutLeaveMainLoop();
}

void Init_GLUT::idleCallback(void) {
	// When nothing's happening, redisplay!
	glutPostRedisplay();
}

void Init_GLUT::displayCallback() {
	if (listener) {
		listener->NotifyBeginFrame();
		listener->NotifyDisplayFrame();
		listener->NotifyEndFrame();
	}
}

void Init_GLUT::reshapeCallback(int width, int height) {
	if (windowInfo.isReshapable == true) {
		if (listener) {
			listener->NotifyReshape(width, height, windowInfo.width, windowInfo.height);
			windowInfo.width = width;
			windowInfo.height = height;
		}
	}
}

void Init_GLUT::closeCallback() {
	Close();
}

void Init_GLUT::PrintOpenGLInfo(const WindowInfo & window, const ContextInfo & context)
{
	const unsigned char* renderer = glGetString(GL_RENDERER);
	const unsigned char* vendor = glGetString(GL_VENDOR);
	const unsigned char* version = glGetString(GL_VERSION);
	cout << "******************************************************************************" << endl;
	cout << "GLUT:\tVendor: " << vendor << endl;
	cout << "GLUT:\tRenderer: " << renderer << endl;
	cout << "GLUT:\tOpenGL version: " << version << endl;
}

void Init_GLUT::SetListener(ISceneListener* iSceneListener)
{
	listener = iSceneListener;
}