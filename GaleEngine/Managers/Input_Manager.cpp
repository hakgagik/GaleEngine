#include "Input_Manager.h"
#include "GL/freeglut.h"
#include <iostream>

using namespace Managers;
using namespace glm;
using namespace std;

Input_Manager::Input_Manager() {}

vec2 Input_Manager::oldMousePos = vec2();
vec2 Input_Manager::mousePos = vec2();
vec2 Input_Manager::mouseDelta = vec2();
bool Input_Manager::arrowDown = false;
bool Input_Manager::arrowUp = false;
bool Input_Manager::arrowLeft = false;
bool Input_Manager::arrowRight = false;
bool Input_Manager::testKey = false;
bool Input_Manager::arrowForward = false;
bool Input_Manager::arrowBack = false;
bool Input_Manager::zoomForward = false;
bool Input_Manager::zoomBack = false;
GLint Input_Manager::mouseLeftButton = GLUT_UP;
GLint Input_Manager::mouseRightButton = GLUT_UP;
GLint Input_Manager::mouseMiddleButton = GLUT_UP;

void Input_Manager::registerCallbacks() {
	glutKeyboardFunc(keyPressed);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMoved);
	glutPassiveMotionFunc(mousePassiveMoved);
}

void Input_Manager::keyPressed(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
		arrowUp = true;
		break;
	case 'a':
		arrowLeft = true;
		break;
	case 's':
		arrowDown = true;
		break;
	case 'd':
		arrowRight = true;
		break;
	case 'r':
		arrowForward = true;
		break;
	case 'f':
		arrowBack = true;
		break;
	case 'v':
		zoomForward = true;
		break;
	case 'c':
		zoomBack = true;
		break;
	case 't':
		testKey = true;
		break;
	default:
		break;
	}
	moveMouse(x, y);
}

void Input_Manager::specialKeyPressed(GLuint key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		arrowUp = true;
		break;
	case GLUT_KEY_DOWN:
		arrowDown = true;
		break;
	case GLUT_KEY_RIGHT:
		arrowRight = true;
		break;
	case GLUT_KEY_LEFT:
		arrowLeft = true;
		break;
	default:
		break;
	}
	moveMouse(x, y);
}

void Input_Manager::moveMouse(int x, int y) {
	mousePos = vec2(x, y);
	//cout << mousePos.x << ", " << mousePos.y << endl;
	mouseDelta = mousePos - oldMousePos;
}

void Input_Manager::mousePressed(GLint button, GLint state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		mouseLeftButton = state;
		break;
	case GLUT_RIGHT_BUTTON:
		mouseRightButton = state;
		break;
	case GLUT_MIDDLE_BUTTON:
		mouseMiddleButton = state;
		break;
	default:
		break;
	}
	moveMouse(x, y);
}

void Input_Manager::mouseMoved(int x, int y) {
	if (mouseLeftButton == GLUT_UP && mouseRightButton == GLUT_UP && mouseMiddleButton == GLUT_UP) {
		cout << "Input Manager: Regular motion func was called even though no mouse button is pressed." << endl;
	}
	else {
		moveMouse(x, y);
	}
}

void Input_Manager::mousePassiveMoved(int x, int y) {
	// Debugging purposes only
	if (mouseLeftButton == GLUT_DOWN || mouseRightButton == GLUT_DOWN || mouseMiddleButton == GLUT_DOWN) {
		cout << "Input Manager: Passive motion func was called even though a mouse button is pressed." << endl;
	}
	moveMouse(x, y);
}

void Input_Manager::update() {
	arrowDown = arrowUp = arrowLeft = arrowRight = testKey = arrowForward = arrowBack = zoomForward = zoomBack = false;
	mouseDelta = vec2(0, 0);
	oldMousePos = mousePos;
}