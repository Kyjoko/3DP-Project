#pragma once

#include "Game.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

//Debug
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

class Display {
private:
	GLFWwindow * window;

	Game* game;


public:
	Display(unsigned int width, unsigned int height, const char* title);
	virtual ~Display();

	void render();

	void mouseMoveCallback(GLFWwindow* window, double x, double y);
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

