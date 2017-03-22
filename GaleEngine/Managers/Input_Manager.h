#pragma once
#include "GL/glew.h"
#include <glm/glm.hpp>

namespace Managers {
	class Input_Manager {
	public:
		static void RegisterCallbacks();
		static void KeyPressed(unsigned char key, int x, int y);
		static void SpecialKeyPressed(GLuint key, int x, int y);
		static void MousePressed(GLint button, GLint state, int x, int y);
		static void MouseMoved(int x, int y);
		static void MousePassiveMoved(int x, int y);
		static void Update();

		static glm::vec2 oldMousePos;
		static glm::vec2 mousePos;
		static glm::vec2 mouseDelta;
		static GLint mouseLeftButton;
		static GLint mouseRightButton;
		static GLint mouseMiddleButton;
		static bool arrowUp;
		static bool arrowDown;
		static bool arrowRight;
		static bool arrowLeft;
		static bool arrowForward;
		static bool arrowBack;
		static bool zoomForward;
		static bool zoomBack;
		static bool togglePhysicsPause;
		static bool stepPhysics;
		static bool toggleFramePause;
		static bool stepFrame;
		static bool testKey;
	private:
		Input_Manager();
		static void moveMouse(int x, int y);
	};
}