// Modified from http://in2gpu.com/opengl-3/
#include "Init_GLUT.h"

using namespace Core;
using namespace Init;

IListener* Init_GLUT::listener = NULL;
WindowInfo Init_GLUT::windowInfo;

void Init_GLUT::init(const WindowInfo & window, const ContextInfo & context, const FramebufferInfo & framebufferInfo)
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

	std::cout << "GLUT: initialized" << std::endl;

	glEnable(GL_DEBUG_OUTPUT);
	// now intialize glew... no idea why this has to be done
	Init_GLEW::Init();
	glDebugMessageCallback(DebugOutput::myCallback, NULL);
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
	printOpenGLInfo(window, context);
}

void Init_GLUT::run() {
	std::cout << "GLUT:\tStart Running " << std::endl;
	glutMainLoop();
}

void Init_GLUT::close() {
	std::cout << "GLUT:\tFinished" << std::endl;
	glutLeaveMainLoop();
}

void Init_GLUT::idleCallback(void) {
	// When nothing's happening, redisplay!
	glutPostRedisplay();
}

void Init_GLUT::displayCallback() {
	if (listener) {
		listener->notifyBeginFrame();
		listener->notifyDisplayFrame();

		glutSwapBuffers();

		listener->notifyEndFrame();
	}
}

void Init_GLUT::reshapeCallback(int width, int height) {
	if (windowInfo.isReshapable == true) {
		if (listener) {
			listener->notifyReshape(width, height, windowInfo.width, windowInfo.height);
			windowInfo.width = width;
			windowInfo.height = height;
		}
	}
}

void Init_GLUT::closeCallback() {
	close();
}

void Init_GLUT::enterFullscreen() {
	glutFullScreen();
}

void Init_GLUT::exitFullscreen() {
	glutLeaveFullScreen();
}

void Init_GLUT::printOpenGLInfo(const WindowInfo & window, const ContextInfo & context)
{
	const unsigned char* renderer = glGetString(GL_RENDERER);
	const unsigned char* vendor = glGetString(GL_VENDOR);
	const unsigned char* version = glGetString(GL_VERSION);
	std::cout << "******************************************************************************" << std::endl;
	std::cout << "GLUT:\tVendor: " << vendor << std::endl;
	std::cout << "GLUT:\tRenderer: " << renderer << std::endl;
	std::cout << "GLUT:\tOpenGL version: " << version << std::endl;
}

void Core::Init::Init_GLUT::setListener(IListener *& iListener)
{
	listener = iListener;
}