#pragma once
#include <GL/glew.h>
#include <string>

namespace Core
{

	class DebugOutput
	{
	public:
		DebugOutput();
		~DebugOutput();
		//if you want you can rename myCallback; This is just an example
		static void __stdcall myCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *data);

	private:
		//Parsing code from OpenGL Shader Language CookBook SE
		//https://github.com/daw42/glslcookbook
		static std::string getStringForType(GLenum type);
		static std::string getStringForSource(GLenum source);
		static std::string getStringForSeverity(GLenum severity);
	};
}