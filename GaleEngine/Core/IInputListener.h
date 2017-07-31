#pragma once
#include "GL/glew.h"

namespace Core {
	class IInputListener {
	public:
		virtual void NotifyKeyDown(char key) = 0;
		virtual void NotifyKeyUp(char key) = 0;
		virtual void NotifySpecialKeyDown(GLint key) = 0;
		virtual void NotifySpecialKeyUp(GLint key) = 0;
		virtual void NotifyMouseDown(GLint button) = 0;
		virtual void NotifyMouseUp(GLint button) = 0;
		virtual void NotifyMouseMove(int dx, int dy) = 0;
		virtual void NotifyMouseWheel(int dz) = 0;
	};
}