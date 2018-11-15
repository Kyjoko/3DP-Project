#include "Display.h"


Display::Display(unsigned int width, unsigned int height, const char * title) {
	//Initialize GLFW
	if (!glfwInit())
		exit(-1);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(-1);
	}
	
	glfwMakeContextCurrent(window);

	glfwSetWindowUserPointer(window, this);
	auto mouse = [](GLFWwindow* w, double x, double y) {
		static_cast<Display*>(glfwGetWindowUserPointer(w))->mouseMoveCallback(w, x, y);
	};
	auto key = [](GLFWwindow* w, int k, int s, int a, int m) {
		static_cast<Display*>(glfwGetWindowUserPointer(w))->keyCallback(w, k, s, a, m);
	};
	//Mouse movement callback
	glfwSetCursorPosCallback(window, mouse);
	//Key callback
	glfwSetKeyCallback(window, key);

	//Initialize ImGui
	ImGui_ImplGlfwGL3_Init(window, false);

	game = new Game(window, width, height);

	//glfwSetTime(0.0);
	double fpsTimer = 0.0;
	while (!glfwWindowShouldClose(window)) {
		double dt = glfwGetTime();
		glfwSetTime(0.0);
		fpsTimer += dt;

		glfwSwapBuffers(window);

		glfwPollEvents();
		
		game->update(dt);
		if (fpsTimer >= 0.5) {
			fpsTimer = 0.0;
			std::string fps = std::string("FPS: " + std::to_string((int)(1.0 / dt)));
			glfwSetWindowTitle(window, fps.c_str());
		}
		render();
	}
}

Display::~Display() {
	delete game;
	glfwTerminate();

}

void Display::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	game->render();
}

void Display::mouseMoveCallback(GLFWwindow* window, double x, double y) {
	game->mouseMoveCallback(window, x, y);
}

void Display::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	game->keyCallback(window, key, scancode, action, mods);
}
