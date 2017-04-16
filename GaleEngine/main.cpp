#include "Core/WindowInfo.h"
#include "Core/ContextInfo.h"
#include "Core/FrameBufferInfo.h"
#include "Core/Init/Init_GLUT.h"
#include "Managers/Scene_Manager.h"
#include <string>

using namespace Core;
using namespace Init;
using namespace Managers;

int main(int argc, char **argv) {
	WindowInfo window(std::string("Gale Engine"), 400, 200, 1920, 1080, true);

	ContextInfo context(4, 3, true);
	FramebufferInfo frameBufferInfo(true, true, true, true);

	Init_GLUT::init(window, context, frameBufferInfo);

	ISceneListener* scene = &(Scene_Manager::Get());
	dynamic_cast<Scene_Manager*>(scene)->SetupTestScene();
	
	Init_GLUT::setListener(scene);

	Init_GLUT::run();

	return 0;
}