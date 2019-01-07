#pragma once
#include "Model.h"
class Terrain
{
public:
	Terrain(glm::vec3 pos, glm::vec2 size, float detail, const char* pathToBitmap = 0);
	virtual ~Terrain();

	Model* model;

	void draw();

};

