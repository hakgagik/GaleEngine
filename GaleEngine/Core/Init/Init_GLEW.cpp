// Modified from http://in2gpu.com/opengl-3/
#include "Init_GLEW.h"
#include "GL/glew.h"
#include <iostream>
using namespace Core::Init;
using namespace std;


void Init_GLEW::Init()
{
	glewExperimental = true;
	if (glewInit() == GLEW_OK) cout << "GLEW: Initialize" << endl;
	if (glewIsSupported("GL_VERSION_4_5")) cout << "GLEW GL_VERSION_4_5 is 4.5\n";
	else cout << "GLEW GL_VERSION_4_5 not supported\n";
}
