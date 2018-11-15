#pragma once
#include <GL\glew.h>
#include <glm.hpp>

#include <fstream>
#include <string>
#include <streambuf>

#include <iostream>

class Shader {
private:
	GLuint gShaderProgram;
	GLuint loadSource(const char* sourcePath, GLint type);
public:
	Shader();
	Shader(const char* vertexSourcePath, const char* fragmetSourcePath);
	Shader(const char* vertexSourcePath, const char* geometrySourcePath, const char* fragmetSourcePath);
	virtual ~Shader();

	void use();

	GLuint getProgram() const;
};

