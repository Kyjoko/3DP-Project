#pragma once

#include "Camera.h"
#include "Object.h"
#include "Shader.h"

#include <iostream>
#include <functional>
#include <queue>
#include <vector>
#include <stdlib.h>
#include <time.h>

//#include <glm.hpp>

class Game {
private:

	GLFWwindow* window;
	unsigned int width;
	unsigned int height;

	Camera* cam;

	ShaderHandler* shaderHandler;

	Object* shadow;
	Object* monkey;
	Object* box;

	Model* terrain;

	std::vector<Object*> particleList;
	float timeElapsed;

	//Debug?
	PointLight* cameraLight;

public:

	Game(GLFWwindow* window, unsigned int width, unsigned int height);
	virtual ~Game();
	
	void update(double dt);
	void render();
	int getHeight();
	int getWidth();
	
	void mouseMoveCallback(GLFWwindow* window, double x, double y);
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

};

