#include "Game.h"

Game::Game(GLFWwindow* window, unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	this->window = window;
	this->timeElapsed = 0;
	std::srand(time(NULL));

	this->particlePos = -10;

	//Initialize GLEW
	glewInit();

	std::cout << glGetString(GL_VERSION) << std::endl;

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	cam = new Camera(width, height, 45.f);

	shaderHandler = new ShaderHandler(cam);

	shaderHandler->addShader(new Shader("ParticleVertexShader.vs", "ParticleFragmentShader.fs"), "Particles");
	shaderHandler->use("Particles");

	shaderHandler->addShader(new Shader("VertexShader.vs", "FragmentShader.fs"), "default_shader");
	shaderHandler->use("default_shader");

	shaderHandler->addShader(new Shader("VertexShader1.vs", "FragmentShader.fs"), "weirdGlow_shader");
	shaderHandler->addShader(new Shader("VertexShader.vs", "GeometryShader.glsl", "FragmentShader.fs"), "Geo");
	shaderHandler->use("Geo");

	//Debug
	monkey = new Object(shaderHandler, "../Resources/Monkey.obj", false);
	box = new Object(shaderHandler, "../Resources/Box1.obj", true);

	box->getTransform()->translate(glm::vec3(4, 0, 2));

	shaderHandler->addLight(PointLight{ glm::vec3(0, 0, 2), glm::vec4(1, 0, 0, 1), 1});		//<--Drog ner radius lite så man kan 
	shaderHandler->addLight(PointLight{ glm::vec3(0, 0, -2), glm::vec4(0, 1, 0, 1), 1.5});	//se texturen utan att bli blind
	shaderHandler->updateLights();

}

Game::~Game() {
	delete cam;
	delete shaderHandler;

	delete monkey;
	delete box;

	for (int i = 0; i < particleList.size(); i++) {
		delete particleList[i];
	}
}

void Game::update(double dt) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (particleList.size() < 2048 && timeElapsed > static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0)))
	{
		if (this->particlePos == 14)
		{
			particlePos = -10;
		}

		particleList.insert(particleList.begin(), new Object(shaderHandler, 5, glm::vec3(rand()%22-10, -15.0f, -10.0f)));
		//particleList.front()->getTransform()->translate(particleList.front()->position);
		
		particlePos += 2;
		timeElapsed = 0;
	}
	for (int i = 0; i < particleList.size(); i++) {
		if (particleList[i]->particleUpdate(particleList[i], dt) == false) {
			particleList[i]->position.y = -15;
			particleList[i]->speed.y = 0;
			particleList[i]->elapsedTime = 0;

			//particleList.erase(particleList.begin()+particleList.size()-1);
		}
		else {
			particleList[i]->getTransform()->translate(particleList[i]->change);
		}
	}

	cam->update(dt);

	timeElapsed += dt;
	
	//particle->getTransform()->rotate(glm::radians(45.0f) * dt, glm::vec3(0.0f, 1.0f, 0.0f));
	//monkey->getTransform()->rotate(glm::radians(45.0f) * dt, glm::vec3(0.0f, 1.0f, 0.0f));
	//monkey->getTransform()->translate((monkey->getTransform()->getPosition() + monkey->getTransform()->getDir() * (float)dt));
	//box->getTransform()->rotate(glm::radians(45.0f) * dt, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Game::render() {
	shaderHandler->use("Geo");
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

	shaderHandler->use("Particles");
	
	for (int i = 0; i < particleList.size(); i++) {
		particleList[i]->draw();
		shaderHandler->updateParticleView(particleList[i]->getTransform()->getMatrix());
		//std::cout << "drawn: " << particleList.size() << std::endl;
	}
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