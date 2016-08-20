// Modified from http://in2gpu.com/opengl-3/
#pragma once
#include "../lib/json.hpp"
#include "GL/glew.h"
#include <string>
#include <map>

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

		std::string readShader(const std::string& filename);
		GLuint createShader(GLenum shaderType, const std::string& source, const std::string& shaderName);
	};
}