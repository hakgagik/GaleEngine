#pragma once
#include "GL/glew.h"
#include <glm/glm.hpp>

namespace Managers {
	class Input_Manager {
	public:
		static void registerCallbacks();
		static void keyPressed(unsigned char key, int x, int y);
		static void specialKeyPressed(GLuint key, int x, int y);
		static void mousePressed(GLint button, GLint state, int x, int y);
		static void mouseMoved(int x, int y);
		static void mousePassiveMoved(int x, int y);
		static void update();

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
		static bool testKey;
	private:
		Input_Manager();
		static void moveMouse(int x, int y);
	};
}