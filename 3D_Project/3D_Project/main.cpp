#include <GL\glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <streambuf>

#include <iostream>

#include <glm.hpp>

//Debug
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

GLuint glShaderProgram = 0;

void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	ImGui_ImplGlfwGL3_NewFrame();

	ImGui::Text("Hello fucking world! :)");


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
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(TriangleVertex), vert, GL_STATIC_DRAW);

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


	glShaderProgram = glCreateProgram();

	glAttachShader(glShaderProgram, vs);
	glAttachShader(glShaderProgram, fs);
	glLinkProgram(glShaderProgram);

	glDetachShader(glShaderProgram, vs);
	glDetachShader(glShaderProgram, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram(glShaderProgram);
}

int main(void)
{
	GLFWwindow* window;

	//Initialize GLFW
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello", NULL, NULL);
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