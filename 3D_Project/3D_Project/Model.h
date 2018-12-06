#pragma once
#define _CRT_SECURE_NO_DEPRECATE

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>

#include <vector>
#include <array>
#include <iostream>
#include <string>

struct TriangleVertex {
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
};

class Model {
private:
	GLuint buffer;
	GLuint tex;

public:
	std::vector<TriangleVertex> vertices;
	float timeToLive; //Determines when to stop drawing
	Model();
	Model(const char* path, bool hasUV);
	virtual ~Model();

	bool load(const char* path, bool has_uv);

	void draw() const;
};

