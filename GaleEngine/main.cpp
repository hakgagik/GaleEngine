#include "Core/Init/Init_GLUT.h"
#include "Managers/Scene_Manager.h"

using namespace Core;
using namespace Init;

int main(int argc, char **argv) {
	WindowInfo window(std::string("Gale Engine"), 400, 200, 800, 600, true);

	ContextInfo context(4, 3, true);
	FramebufferInfo frameBufferInfo(true, true, true, true);

	Init_GLUT::init(window, context, frameBufferInfo);

	IListener* scene = new Managers::Scene_Manager();
	Init_GLUT::setListener(scene);

	Init_GLUT::run();

	delete scene;
	return 0;
}