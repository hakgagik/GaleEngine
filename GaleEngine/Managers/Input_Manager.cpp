#include "Input_Manager.h"
#include "../Core/IInputListener.h"
#include "GL/freeglut.h"
#include <iostream>

using namespace Managers;
using namespace Core;
using namespace glm;
using namespace std;

Input_Manager Input_Manager::instance;

Input_Manager::Input_Manager() {}

Input_Manager& Input_Manager::Get() {
	return instance;
}

void Input_Manager::Init() {
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMoved);
	glutPassiveMotionFunc(mouseMoved);
}

void Input_Manager::keyDown(unsigned char key, int x, int y) {
	checkAndFireIfMouseMoved(x, y);
	instance.KeyStates[key] = true;
	for (IInputListener* listener : instance.KeyDownListeners)
	{
		listener->NotifyKeyDown(key);
	}
}

void Input_Manager::keyUp(unsigned char key, int x, int y) {
	checkAndFireIfMouseMoved(x, y);
	instance.KeyStates[key] = false;
	for (IInputListener* listener : instance.KeyDownListeners)
	{
		listener->NotifyKeyUp(key);
	}
}

void Input_Manager::specialKeyDown(GLint key, int x, int y) {
	checkAndFireIfMouseMoved(x, y);
	instance.SpecialKeyStates[key] = true;
	for (IInputListener* listener : instance.KeyDownListeners)
	{
		listener->NotifySpecialKeyDown(key);
	}
}

void Input_Manager::specialKeyUp(GLint key, int x, int y) {
	checkAndFireIfMouseMoved(x, y);
	instance.SpecialKeyStates[key] = false;
	for (IInputListener* listener : instance.KeyDownListeners)
	{
		listener->NotifySpecialKeyUp(key);
	}
}

void Input_Manager::mouseButton(GLint button, GLint state, int x, int y) {
	checkAndFireIfMouseMoved(x, y);
	bool bState = false;
	if (button < 3) {
		if (state == GLUT_DOWN) {
			bState = true;
		}
		switch (button) {
		case GLUT_LEFT_BUTTON:
			instance.MouseButtonStates.x = bState;
			break;
		case GLUT_MIDDLE_BUTTON:
			instance.MouseButtonStates.y = bState;
			break;
		case GLUT_RIGHT_BUTTON:
			instance.MouseButtonStates.z = bState;
			break;
		}
		if (bState) {
			for (IInputListener* listener : instance.KeyDownListeners)
			{
				listener->NotifyMouseDown(button);
			}
		}
		else {
			for (IInputListener* listener : instance.KeyDownListeners)
			{
				listener->NotifyMouseUp(button);
			}
		}
	}
	else if (button == 3 || button == 4) {
		if (state == GLUT_DOWN) return;
		int dz;
		if (button == 3) dz = 1;
		else if (button == 4) dz = -1;
		for (IInputListener* listener : instance.KeyDownListeners)
		{
			listener->NotifyMouseWheel(dz);
		}
	}
}

void Input_Manager::mouseMoved(int x, int y) {
	ivec2 dx = ivec2(x, y) - instance.MousePos;
	for (IInputListener* listener : instance.KeyDownListeners)
	{
		listener->NotifyMouseMove(dx.x, dx.y);
	}
	instance.MousePos.x = x;
	instance.MousePos.y = y;
}

void Input_Manager::checkAndFireIfMouseMoved(int x, int y) {
	if (instance.MousePos.x != x || instance.MousePos.y != y) {
		mouseMoved(x, y);
	}
}

//vec2 Input_Manager::oldMousePos = vec2();
//vec2 Input_Manager::mousePos = vec2();
//vec2 Input_Manager::mouseDelta = vec2();
//bool Input_Manager::arrowDown = false;
//bool Input_Manager::arrowUp = false;
//bool Input_Manager::arrowLeft = false;
//bool Input_Manager::arrowRight = false;
//bool Input_Manager::testKey = false;
//bool Input_Manager::arrowForward = false;
//bool Input_Manager::arrowBack = false;
//bool Input_Manager::zoomForward = false;
//bool Input_Manager::zoomBack = false;
//bool Input_Manager::togglePhysicsPause = false;
//bool Input_Manager::stepPhysics = false;
//bool Input_Manager::toggleFramePause = false;
//bool Input_Manager::stepFrame = false;
//GLint Input_Manager::mouseLeftButton = GLUT_UP;
//GLint Input_Manager::mouseRightButton = GLUT_UP;
//GLint Input_Manager::mouseMiddleButton = GLUT_UP;
//
//void Input_Manager::Initialize() {
//	glutKeyboardFunc(KeyPressed);
//	glutSpecialFunc(SpecialKeyPressed);
//	glutMouseFunc(MousePressed);
//	glutMotionFunc(mouseMoved);
//	glutPassiveMotionFunc(MousePassiveMoved);
//}
//
//void Input_Manager::KeyPressed(unsigned char key, int x, int y) {
//	switch (key)
//	{
//	case 'w':
//		arrowUp = true;
//		break;
//	case 'a':
//		arrowLeft = true;
//		break;
//	case 's':
//		arrowDown = true;
//		break;
//	case 'd':
//		arrowRight = true;
//		break;
//	case 'r':
//		arrowForward = true;
//		break;
//	case 'f':
//		arrowBack = true;
//		break;
//	case 'v':
//		zoomForward = true;
//		break;
//	case 'c':
//		zoomBack = true;
//		break;
//	case 't':
//		testKey = true;
//		break;
//	case 'p':
//		togglePhysicsPause = true;
//		break;
//	case 'o':
//		stepPhysics = true;
//		break;
//	case 'l':
//		toggleFramePause = true;
//		break;
//	case 'k':
//		stepFrame = true;
//		break;
//	default:
//		break;
//	}
//	moveMouse(x, y);
//}
//
//void Input_Manager::SpecialKeyPressed(GLint key, int x, int y) {
//	switch (key)
//	{
//	case GLUT_KEY_UP:
//		arrowUp = true;
//		break;
//	case GLUT_KEY_DOWN:
//		arrowDown = true;
//		break;
//	case GLUT_KEY_RIGHT:
//		arrowRight = true;
//		break;
//	case GLUT_KEY_LEFT:
//		arrowLeft = true;
//		break;
//	default:
//		break;
//	}
//	moveMouse(x, y);
//}
//
//void Input_Manager::moveMouse(int x, int y) {
//	mousePos = vec2(x, y);
//	//cout << mousePos.x << ", " << mousePos.y << endl;
//	mouseDelta = mousePos - oldMousePos;
//}
//
//void Input_Manager::MousePressed(GLint button, GLint state, int x, int y) {
//	switch (button)
//	{
//	case GLUT_LEFT_BUTTON:
//		mouseLeftButton = state;
//		break;
//	case GLUT_RIGHT_BUTTON:
//		mouseRightButton = state;
//		break;
//	case GLUT_MIDDLE_BUTTON:
//		mouseMiddleButton = state;
//		break;
//	default:
//		break;
//	}
//	moveMouse(x, y);
//}
//
//void Input_Manager::mouseMoved(int x, int y) {
//	if (mouseLeftButton == GLUT_UP && mouseRightButton == GLUT_UP && mouseMiddleButton == GLUT_UP) {
//		cout << "Input Manager: Regular motion func was called even though no mouse button is pressed." << endl;
//	}
//	else {
//		moveMouse(x, y);
//	}
//}
//
//void Input_Manager::MousePassiveMoved(int x, int y) {
//	// Debugging purposes only
//	if (mouseLeftButton == GLUT_DOWN || mouseRightButton == GLUT_DOWN || mouseMiddleButton == GLUT_DOWN) {
//		cout << "Input Manager: Passive motion func was called even though a mouse button is pressed." << endl;
//	}
//	moveMouse(x, y);
//}
//
//void Input_Manager::Update() {
//	arrowDown = arrowUp = arrowLeft = arrowRight = testKey = arrowForward = arrowBack = zoomForward = zoomBack = stepPhysics = togglePhysicsPause = stepFrame = toggleFramePause = false;
//	mouseDelta = vec2(0, 0);
//	oldMousePos = mousePos;
//}