#pragma once
#include "Shaderhandler.h"
#include "Terrain.h"
#include "Object.h"
#include <gtc\matrix_transform.hpp>
#include <GL\glew.h>
#include <glm.hpp>

class Shadow
{
public:
	Shadow(ShaderHandler* shader);
	~Shadow();

	void renderDepth(ShaderHandler* shader, std::vector<Object*> listA, std::vector<Terrain*> listB);
	void initDepth(ShaderHandler * shader);
	glm::mat4 lightMatrix();
	
private:
	GLuint depthTex;
	GLuint depthMapFBO;
	unsigned int SHADOW_WIDTH;
	unsigned int SHADOW_HEIGHT;
};

