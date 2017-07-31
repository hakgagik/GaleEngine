#pragma once
#include "GL/glew.h"
#include <unordered_map>
#include <glm/glm.hpp>
#include <vector>

namespace Core {
	class IInputListener;
}

namespace Managers {
	class Input_Manager {
	public:
		static Input_Manager& Get();
		glm::bvec3 MouseButtonStates;
		glm::ivec2 MousePos;
		std::unordered_map<unsigned char, bool> KeyStates;
		std::unordered_map<GLint, bool> SpecialKeyStates;

		std::vector<Core::IInputListener*> KeyDownListeners;
		std::vector<Core::IInputListener*> KeyUpListeners;
		std::vector<Core::IInputListener*> SpecialKeyDownListeners;
		std::vector<Core::IInputListener*> SpecialKeyUpListeners;
		std::vector<Core::IInputListener*> MouseDownListeners;
		std::vector<Core::IInputListener*> MouseUpListeners;
		std::vector<Core::IInputListener*> MouseMoveListeners;
		std::vector<Core::IInputListener*> MouseWheelListeners;

		void Init();
	private:
		static void keyDown(unsigned char key, int x, int y);
		static void keyUp(unsigned char key, int x, int y);
		static void specialKeyDown(GLint key, int x, int y);
		static void specialKeyUp(GLint key, int x, int y);
		static void mouseButton(GLint button, GLint state, int x, int y);
		static void mouseMoved(int x, int y);
		static void checkAndFireIfMouseMoved(int x, int y);

		static Input_Manager instance;



		//static glm::vec2 oldMousePos;
		//static glm::vec2 mousePos;
		//static glm::vec2 mouseDelta;
		//static GLint mouseLeftButton;
		//static GLint mouseRightButton;
		//static GLint mouseMiddleButton;
		//static bool arrowUp;
		//static bool arrowDown;
		//static bool arrowRight;
		//static bool arrowLeft;
		//static bool arrowForward;
		//static bool arrowBack;
		//static bool zoomForward;
		//static bool zoomBack;
		//static bool togglePhysicsPause;
		//static bool stepPhysics;
		//static bool toggleFramePause;
		//static bool stepFrame;
		//static bool testKey;

		Input_Manager();
		//static void moveMouse(int x, int y);
	};
}