#include "Game.h"

Game::Game(GLFWwindow* window, unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	this->window = window;

	//Initialize GLEW
	glewInit();

	std::cout << glGetString(GL_VERSION) << std::endl;

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	cam = new Camera(width, height, 45.f);

	shaderHandler = new ShaderHandler(cam);
	shaderHandler->addShader(new Shader("VertexShader.vs", "FragmentShader.fs"), "default_shader");
	shaderHandler->use("default_shader");

	shaderHandler->addShader(new Shader("VertexShader1.vs", "FragmentShader.fs"), "weirdGlow_shader");
	shaderHandler->addShader(new Shader("VertexShader.vs", "GeometryShader.glsl", "FragmentShader.fs"), "Geo");
	shaderHandler->use("Geo");
	//Debug
	monkey = new Object(shaderHandler, "../Resources/Monkey.obj", false);
	box = new Object(shaderHandler, "../Resources/Box1.obj", true);
	
	box->getTransform()->translate(glm::vec3(4, 0, 2));

	shaderHandler->addLight(PointLight{ glm::vec3(0, 0, 2), glm::vec4(1, 0, 0, 1), 5 });
	shaderHandler->addLight(PointLight{ glm::vec3(0, 0, -2), glm::vec4(0, 1, 0, 1), 1 });
	shaderHandler->updateLights();
}

Game::~Game() {
	delete cam;
	delete shaderHandler;

	delete monkey;
	delete box;
}

void Game::update(double dt) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	cam->update(dt);
	
	//monkey->getTransform()->rotate(glm::radians(45.0f) * dt, glm::vec3(0.0f, 1.0f, 0.0f));
	//monkey->getTransform()->translate((monkey->getTransform()->getPosition() + monkey->getTransform()->getDir() * (float)dt));
	//box->getTransform()->rotate(glm::radians(45.0f) * dt, glm::vec3(0.0f, 1.0f, 0.0f));

}

void Game::render() {
	//shaderHandler->use("default_shader");
	shaderHandler->updateView();
	
	box->draw();

	//shaderHandler->use("weirdGlow_shader");
	//shaderHandler->updateView();
	
	monkey->draw();
	/*
	ImGui_ImplGlfwGL3_NewFrame();

	//ImGui::SliderFloat("FOV", &FOV, 10.f, 180.f);

	//ImGui::Button("Hello", ImVec2(50, 50));

	//ImGui::Text(std::to_string(cam->m_viewDir.y).c_str());

	ImGui::Render();
	*/
}

void Game::mouseMoveCallback(GLFWwindow* window, double x, double y) {
	cam->handleMouseMovement(glm::vec2(x, y));
}

void Game::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	cam->handleKeyInput(key, action);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}