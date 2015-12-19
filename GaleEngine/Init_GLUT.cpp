#include "Init_GLUT.h"

using namespace Core::Init;

void Core::Init::Init_GLUT::init(const Core::WindowInfo & window, const Core::ContextInfo & context, const Core::FramebufferInfo & framebufferInfo)
{
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
	glutCreateWindow(window.name.c_str()); // finally create the window

	std::cout << "GLUT: initialized" << std::endl;

	// set the callback functions
	glutIdleFunc(idleCallback);
	glutCloseFunc(closeCallback);
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);

	// now intialize glew... no idea why this has to be done
	Init::Init_GLEW::Init();

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1);
	glutSwapBuffers();
}

void Init_GLUT::reshapeCallback(int windth, int height) {

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

void Core::Init::Init_GLUT::printOpenGLInfo(const Core::WindowInfo & window, const Core::ContextInfo & context)
{
	const unsigned char* renderer = glGetString(GL_RENDERER);
	const unsigned char* vendor = glGetString(GL_VENDOR);
	const unsigned char* version = glGetString(GL_VERSION);
	std::cout << "******************************************************************************" << std::endl;
	std::cout << "GLUT:\tVendor: " << vendor << std::endl;
	std::cout << "GLUT:\tRenderer: " << renderer << std::endl;
	std::cout << "GLUT:\tOpenGL version: " << version << std::endl;
}
