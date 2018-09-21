#define _CRT_SECURE_NO_DEPRECATE

#include <GL\glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <streambuf>

#include <iostream>

#include <vector>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include "Camera.h"

//Debug
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

const int WIDTH = 640;
const int HEIGHT = 480;

glm::mat4 mat_world = glm::mat4(1.f);
glm::mat4 mat_view = glm::lookAt(
	glm::vec3(0, 0, -2), //position
	glm::vec3(0, 0, -1), //look direction
	glm::vec3(0, 1, 0) //up
);
glm::mat4 mat_projection = glm::mat4(1.f);

GLFWwindow* window;
GLuint gShaderProgram = 0;

Camera* cam;

struct TriangleVertex {
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
};

float FOV = 45.f;
void render(unsigned int vertices) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam->update();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	mat_view = cam->getMatrix();

	mat_projection = glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 20.f);
	GLuint worldLoc = glGetUniformLocation(gShaderProgram, "mat_world");
	glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &mat_world[0][0]);

	GLuint viewLoc = glGetUniformLocation(gShaderProgram, "mat_view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &mat_view[0][0]);

	GLuint projLoc = glGetUniformLocation(gShaderProgram, "mat_projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &mat_projection[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, vertices);

	/*
	ImGui_ImplGlfwGL3_NewFrame();

	//ImGui::SliderFloat("FOV", &FOV, 10.f, 180.f);

	//ImGui::Button("Hello", ImVec2(50, 50));

	//ImGui::Text(std::to_string(cam->m_viewDir.y).c_str());

	ImGui::Render();
	*/
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

bool loadModel(const char* path,
	std::vector<glm::vec3> &out_vertices,
	std::vector<glm::vec2> &out_uvs,
	std::vector<glm::vec3> &out_normals,
	bool has_uv) {

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vert;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* objFile = fopen(path, "r");
	if (objFile == NULL) return false;
	while(true) {
		char lineStart[256];
		int res = fscanf(objFile, "%s", lineStart);
		if (res == EOF) break;

		if (strcmp(lineStart, "v") == 0) { //pos
			glm::vec3 v;
			fscanf(objFile, "%f %f %f\n", &v.x, &v.y, &v.z);
			temp_vert.push_back(v);
		}
		else if (strcmp(lineStart, "vt") == 0) { //uvs
			glm::vec2 u;
			fscanf(objFile, "%f %f\n", &u.x, &u.y);
			temp_uvs.push_back(u);
		}
		else if (strcmp(lineStart, "vn") == 0) { //normals
			glm::vec3 n;
			fscanf(objFile, "%f %f %f\n", &n.x, &n.y, &n.z);
			temp_normals.push_back(n);
		}
		else if (strcmp(lineStart, "f") == 0) { //faces
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			if (has_uv) {

				int match = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0],
					&uvIndex[0],
					&normalIndex[0],
					&vertexIndex[1],
					&uvIndex[1],
					&normalIndex[1],
					&vertexIndex[2],
					&uvIndex[2],
					&normalIndex[2]
					);
				if (match != 9) return false;
			
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);

			}
			else
			{
				int match = fscanf(objFile, "%d//%d %d//%d %d//%d\n",
					&vertexIndex[0],
					&normalIndex[0],
					&vertexIndex[1],
					&normalIndex[1],
					&vertexIndex[2],
					&normalIndex[2]
				);				
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			
		}
	}
	
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vert[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}

}

bool loadObjModel(const char* path, std::vector<TriangleVertex>& out_vertices, bool has_uv) {

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vert;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* objFile = fopen(path, "r");
	if (objFile == NULL) return false;
	while (true) {
		char lineStart[256];
		int res = fscanf(objFile, "%s", lineStart);
		if (res == EOF) break;

		if (strcmp(lineStart, "v") == 0) { //pos
			glm::vec3 v;
			fscanf(objFile, "%f %f %f\n", &v.x, &v.y, &v.z);
			temp_vert.push_back(v);
		}
		else if (strcmp(lineStart, "vt") == 0) { //uvs
			glm::vec2 u;
			fscanf(objFile, "%f %f\n", &u.x, &u.y);
			temp_uvs.push_back(u);
		}
		else if (strcmp(lineStart, "vn") == 0) { //normals
			glm::vec3 n;
			fscanf(objFile, "%f %f %f\n", &n.x, &n.y, &n.z);
			temp_normals.push_back(n);
		}
		else if (strcmp(lineStart, "f") == 0) { //faces
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			if (has_uv) {

				int match = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0],
					&uvIndex[0],
					&normalIndex[0],
					&vertexIndex[1],
					&uvIndex[1],
					&normalIndex[1],
					&vertexIndex[2],
					&uvIndex[2],
					&normalIndex[2]
				);
				if (match != 9) return false;

				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);

			}
			else
			{
				int match = fscanf(objFile, "%d//%d %d//%d %d//%d\n",
					&vertexIndex[0],
					&normalIndex[0],
					&vertexIndex[1],
					&normalIndex[1],
					&vertexIndex[2],
					&normalIndex[2]
				);
				if (match != 6) return false;

			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		TriangleVertex tmp;
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vert[vertexIndex - 1];
		tmp.pos = vertex;

		if(uvIndices.size() > i){
			unsigned int uvIndex = uvIndices[i];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			tmp.uv = uv;
		}
		if (normalIndices.size() > i) {
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 normal = temp_normals[normalIndex - 1];
			tmp.normal = normal;
		}

		out_vertices.push_back(tmp);
	}
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

	//Check for errors
	int result;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);
		char message[256];
		glGetShaderInfoLog(vs, length, &length, message);
		std::cout << "Faild to compile vertex shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(vs);
	}

	//Fragment Shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	shaderFile.open("FragmentShader.glsl");
	shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();

	shaderTextPtr = shaderText.c_str();
	glShaderSource(fs, 1, &shaderTextPtr, nullptr);
	glCompileShader(fs);

	//Check for errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);
		char message[256];
		glGetShaderInfoLog(vs, length, &length, message);
		std::cout << "Faild to compile fragment shader" << std::endl;
		std::cout << message << std::endl;
	}

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

static void mouseMoveCallback(GLFWwindow* window, double x, double y) {

	cam->handleMouseMovement(glm::vec2(x, y));
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	cam->handleKeyInput(key, action);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main(void) {

	//Initialize GLFW
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	//Mouse movement callback
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	//Key callback
	glfwSetKeyCallback(window, keyCallback);

	//Initialize GLEW
	glewInit();

	//Initialize ImGui
	ImGui_ImplGlfwGL3_Init(window, false);

	std::cout << glGetString(GL_VERSION) << std::endl;
	
	setupShaders();
	


	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	
	//createTriangleData();


	// Read our .obj file
	/*
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	if (!loadModel("../Resources/Monkey.obj", vertices, uvs, normals, false)) {
		std::cout << "Could not load file" << std::endl;
	}
	std::cout << "Loaded " << vertices.size() << " vertices" << std::endl;
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(glm::vec3),
		&vertices[0],
		GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	*/

	std::vector<TriangleVertex> vertices;
	if (!loadObjModel("../Resources/Monkey.obj", vertices, false)) {
		std::cout << "Could not load file" << std::endl;
	}
	std::cout << "Loaded " << vertices.size() << " vertices" << std::endl;
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(TriangleVertex),
		&vertices[0],
		GL_DYNAMIC_DRAW);

	//Pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), 0);
	//UV
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (const void*) sizeof(glm::vec3));
	//Normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (const void*) (sizeof(glm::vec3) + sizeof(glm::vec2)));

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	cam = new Camera();

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);

		glfwPollEvents();

		render(vertices.size());
	}

	glDeleteProgram(gShaderProgram);
	glfwTerminate();
	delete cam;
	return 0;
}