#include "Shader.h"

Shader::Shader() {

}

Shader::Shader(const char * vertexSourcePath, const char * fragmetSourcePath) {
	gShaderProgram = glCreateProgram();
	GLuint vs = loadSource(vertexSourcePath, GL_VERTEX_SHADER);
	GLuint fs = loadSource(fragmetSourcePath, GL_FRAGMENT_SHADER);
	
	glLinkProgram(gShaderProgram);
	
	glDetachShader(gShaderProgram, vs);
	glDetachShader(gShaderProgram, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	
}

Shader::Shader(const char * vertexSourcePath, const char * geometrySourcePath, const char * fragmetSourcePath) {
	gShaderProgram = glCreateProgram();
	GLuint vs = loadSource(vertexSourcePath, GL_VERTEX_SHADER);
	GLuint gs = loadSource(geometrySourcePath, GL_GEOMETRY_SHADER);
	GLuint fs = loadSource(fragmetSourcePath, GL_FRAGMENT_SHADER);

	glLinkProgram(gShaderProgram);

	glDetachShader(gShaderProgram, vs);
	glDetachShader(gShaderProgram, gs);
	glDetachShader(gShaderProgram, fs);
	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);
}

Shader::~Shader() {
	glDeleteProgram(gShaderProgram);
}

GLuint Shader::loadSource(const char* sourcePath, GLint type) {
	GLuint s = glCreateShader(type);

	std::ifstream shaderFile(sourcePath);
	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();

	const char* shaderTextPtr = shaderText.c_str();
	glShaderSource(s, 1, &shaderTextPtr, nullptr);
	glCompileShader(s);

	//Check for errors
	int result;
	glGetShaderiv(s, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(s, GL_INFO_LOG_LENGTH, &length);
		char message[256];
		glGetShaderInfoLog(s, length, &length, message);
		std::cout << "Failed to compile " << type << " shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(s);
	}

	glAttachShader(gShaderProgram, s);

	return s;
}

void Shader::use() {
	glUseProgram(gShaderProgram);
}

GLuint Shader::getProgram() const {
	return gShaderProgram;
}

