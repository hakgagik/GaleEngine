#include "Init_GLUT.h"
using namespace Core;
using namespace Init;

void main(int argc, char **argv) {
	WindowInfo window(std::string("Gale Engine"), 400, 200, 800, 600, true);
	ContextInfo context(4, 5, true);
	FramebufferInfo frameBufferInfo(true, true, true, true);
	Init_GLUT::init(window, context, frameBufferInfo);
	Init_GLUT::run();
}