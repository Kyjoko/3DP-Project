#pragma once
#include "Shaderhandler.h"
#include "Model.h"
#include <gtc\matrix_transform.hpp>
#include <GL\glew.h>
#include <glm.hpp>

class Shadow
{
public:
	Shadow(ShaderHandler* shader);
	~Shadow();

	void depthMap(ShaderHandler* shader);
	void renderDepth(ShaderHandler* shader, Model* model);
	glm::mat4 lightMatrix();
	
private:
	unsigned int depthMapFBO;
	unsigned int SHADOW_WIDTH;
	unsigned int SHADOW_HEIGHT;
};

