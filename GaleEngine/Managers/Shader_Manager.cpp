// Modified from http://in2gpu.com/opengl-3/
#include "Shader_Manager.h"

using namespace Managers;

std::map<std::string, GLuint> Shader_Manager::programs;

const GLuint Managers::Shader_Manager::GetShader(const std::string & shaderName)
{
	return programs.at(shaderName);
}

Shader_Manager::Shader_Manager(void) {}
Shader_Manager::~Shader_Manager(void) {
	std::map<std::string, GLuint>::iterator i;
	for (i = programs.begin(); i != programs.end(); ++i) {
		GLuint pr = i->second;
		glDeleteProgram(pr);
	}
	programs.clear();
}

void Managers::Shader_Manager::CreateProgram(const std::string & shaderName, const std::string & VertexShaderFilename, const std::string & FragmentShaderFilename)
{
	if (programs.count(shaderName) > 0) {
		std::cout << "Error: Program " << shaderName.c_str() << " already exists.";
		return;
	}
	// read the code for the shaders
	std::string vertex_shader_code = ReadShader(VertexShaderFilename);
	std::string fragment_shader_code = ReadShader(FragmentShaderFilename);

	// compile the shaders
	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, VertexShaderFilename);
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, FragmentShaderFilename);

	int link_result = 0;
	// put 'em together into a program
	GLuint program = glCreateProgram(); // ask GL to create a blank program for us;
	glAttachShader(program, vertex_shader); // attach the shaders
	glAttachShader(program, fragment_shader);

	// Link em!
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result); // ask GL to tell us how the compile went
	//check for link errors
	if (link_result == GL_FALSE) {
		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return;
	}
	programs[shaderName] = program;
}

std::string Managers::Shader_Manager::ReadShader(const std::string & filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in); // create a filestream
	if (!file.good()) { // make sure it's good
		std::cout << "Can't read file " << filename.c_str() << std::endl;
		std::terminate();
	}
	file.seekg(0, std::ios::end); // go to the end of the file so we can check size
	shaderCode.resize((unsigned int)file.tellg()); // read the end position, determining the file size
	file.seekg(0, std::ios::beg); // go back to the beginning
	file.read(&shaderCode[0], shaderCode.size()); // read the entire shader into the shaderCode string
	file.close(); // close the file
	return shaderCode;
}

GLuint Managers::Shader_Manager::CreateShader(GLenum shaderType, const std::string & source, const std::string & shaderName)
{
	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType); // ask GL to create room for the shader and give us a GLuint to reference it by
	const char *shader_code_ptr = source.c_str(); // create a pointer to the first char in the shader source
	const int shader_code_size = source.size(); // size of the shader source

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size); // tell GL where to look for the shader source
	glCompileShader(shader); // compile the shader from the source we just gave
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result); // get the status of the compile

	if (compile_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length); // get the length of the info log!
		std::vector<char> shader_log(info_log_length); // prepare a vector of chars to store the info log
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]); // ask GL for info on the compilation failure
		std::cout << "ERROR compiling shader: " << shaderName.c_str() << std::endl << &shader_log[0] << std::endl; // print out the error
		return 0;
	}
	return shader;
}
