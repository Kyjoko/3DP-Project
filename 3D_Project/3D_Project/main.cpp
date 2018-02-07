#include <GL\glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <streambuf>

#include <iostream>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

//Debug
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

const int WIDTH = 640;
const int HEIGHT = 480;

glm::mat4 mat_world = glm::mat4(1.f);
glm::mat4 mat_view = glm::mat4(1.f);
glm::mat4 mat_projection = glm::mat4(1.f);

GLuint gShaderProgram = 0;

void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	mat_world = glm::rotate(mat_world, 0.01f, glm::vec3(0, 1, 0));

	mat_view = glm::lookAt(
		glm::vec3(0, 0, -2),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	mat_projection = glm::perspective(3.141592f * 0.45f, (float)WIDTH / (float)HEIGHT, 0.1f, 20.f);

	GLuint worldLoc = glGetUniformLocation(gShaderProgram, "mat_world");
	glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &mat_world[0][0]);

	GLuint viewLoc = glGetUniformLocation(gShaderProgram, "mat_view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &mat_view[0][0]);

	GLuint projLoc = glGetUniformLocation(gShaderProgram, "mat_projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &mat_projection[0][0]);


	glDrawArrays(GL_TRIANGLES, 0, 3);

	ImGui_ImplGlfwGL3_NewFrame();

	ImGui::Text("v1x:");


	ImGui::Render();
}

void createTriangleData() {

	//Triangle data//
	struct TriangleVertex {
		float x, y, z;

	};

	TriangleVertex vert[3] = {
		{ 0.0f, 0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f },
		{ 0.5f, -0.5f, 0.0f },
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(TriangleVertex), vert, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), 0);

}

void setupShaders() {
	//Vertex Shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);

	std::ifstream shaderFile("VertexShader.glsl");
	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();

	const char* shaderTextPtr = shaderText.c_str();
	glShaderSource(vs, 1, &shaderTextPtr, nullptr);
	glCompileShader(vs);

	//Fragment Shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	shaderFile.open("FragmentShader.glsl");
	shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();

	shaderTextPtr = shaderText.c_str();
	glShaderSource(fs, 1, &shaderTextPtr, nullptr);
	glCompileShader(fs);


	gShaderProgram = glCreateProgram();

	glAttachShader(gShaderProgram, vs);
	glAttachShader(gShaderProgram, fs);
	glLinkProgram(gShaderProgram);

	glDetachShader(gShaderProgram, vs);
	glDetachShader(gShaderProgram, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram(gShaderProgram);
}

int main(void)
{
	GLFWwindow* window;

	//Initialize GLFW
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	
	//Initialize GLEW
	glewInit();

	//Initialize ImGui
	ImGui_ImplGlfwGL3_Init(window, false);

	std::cout << glGetString(GL_VERSION) << std::endl;

	setupShaders();
	
	createTriangleData();

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);

		glfwPollEvents();

		render();
	}

	glfwTerminate();
	return 0;
}