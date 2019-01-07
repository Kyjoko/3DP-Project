#pragma once
#define _CRT_SECURE_NO_DEPRECATE

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include "../SOIL/src/SOIL.h" //Simple Open Image Lib
#include "imageloader.h"

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
	GLuint tex[2];

public:
	std::vector<TriangleVertex> vertices;
	float timeToLive; //Determines when to stop drawing
	Model();
	Model(glm::vec3 pos, int width, int height, float tileSize, const char* path); //Terrain
	Model(const char* path, bool hasUV);
	virtual ~Model();

	bool load(const char* path, bool has_uv);

	void draw() const;
	void drawParticle() const;
	void drawDepth() const;
	GLuint* getTex();

	float getPixel(Image* image, int x, int y); //returns a value between -1.f and 1.f depending on the color of th given pixel
	glm::vec3 getNormal(Image* image, float x, float z); //Calculates and returns the normal based on the image
};

