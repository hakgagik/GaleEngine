// Modified from http://in2gpu.com/opengl-3/
#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "../lib/glew/glew.h"
#include "../lib/freeglut/freeglut.h"

namespace Managers
{
	class Shader_Manager {
	public:
		static const GLuint GetShader(const std::string&);

		Shader_Manager(void);
		~Shader_Manager(void);

		void CreateProgram(const std::string& shaderName, const std::string& VertexShaderFilename, const std::string& FragmentShaderFilename);
	private:
		static std::map<std::string, GLuint> programs;

		std::string ReadShader(const std::string& filename);
		GLuint CreateShader(GLenum shaderType, const std::string& source, const std::string& shaderName);
	};
}