#include "ShaderHandler.h"

ShaderHandler::ShaderHandler(Camera* cam) {
	this->cam = cam;
}


ShaderHandler::~ShaderHandler() {
	for (size_t i = 0; i < lights.size(); i++) {
		delete lights[i];
	}

	for (auto const& sh : shaders) {
		delete shaders.at(sh.first);
	}
	shaders.clear();
}

void ShaderHandler::updateView(){
	setUniformMatrix4(cam->getTransform()->getMatrix(), "mat_view");
}

void ShaderHandler::updateParticleView(glm::mat4 m) {
	glm::mat4 mat_view = cam->getTransform()->getMatrix();
	glm::mat4 mat_world = m;
	glm::mat4 mat_worldView;
	mat_world[0][0] = mat_view[0][0];
	mat_world[0][1] = mat_view[1][0];
	mat_world[0][2] = mat_view[2][0];
	mat_world[1][0] = mat_view[0][1];
	mat_world[1][1] = mat_view[1][1];
	mat_world[1][2] = mat_view[2][1];
	mat_world[2][0] = mat_view[0][2];
	mat_world[2][1] = mat_view[1][2];
	mat_world[2][2] = mat_view[2][2];
	mat_worldView = mat_view * mat_world;
	setUniformMatrix4(mat_worldView, "mat_view");
}


void ShaderHandler::setUniformMatrix4(glm::mat4 mat, const char* uniformName) {
	GLuint loc = glGetUniformLocation(getActiveShader()->getProgram(), uniformName);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void ShaderHandler::setUniformVec4(glm::vec4 vec, const char * uniformName) {
	GLuint loc = glGetUniformLocation(getActiveShader()->getProgram(), uniformName);
	glUniform4fv(loc, 1, &vec[0]);
}

void ShaderHandler::setUniformVec3(glm::vec3 vec, const char * uniformName) {
	GLuint loc = glGetUniformLocation(getActiveShader()->getProgram(), uniformName);
	glUniform3fv(loc, 1, &vec[0]);
}

void ShaderHandler::setUniformFloat(float f, const char * uniformName) {
	GLuint loc = glGetUniformLocation(getActiveShader()->getProgram(), uniformName);
	glUniform1f(loc, f);
}

void ShaderHandler::setUniformInt(int i, const char * uniformName) {
	GLuint loc = glGetUniformLocation(getActiveShader()->getProgram(), uniformName);
	glUniform1i(loc, i);
}

std::string ShaderHandler::getActive() const {
	return active;
}

Shader* ShaderHandler::getActiveShader() const {
	return shaders.at(active);
}

void ShaderHandler::use(std::string shaderName) {
	if (active != shaderName) { //if not already in use
		shaders.at(shaderName)->use();
		active = shaderName;
		setUniformMatrix4(cam->getProjection(), "mat_projection");
		updateLights();
	}
}

void ShaderHandler::addShader(Shader* shader, std::string name) {
	shaders.insert(std::pair<std::string, Shader*>(name, shader));
}

void ShaderHandler::addLight(PointLight* light) {
	lights.push_back(light);
}

void ShaderHandler::updateLights() {
	for (size_t i = 0; i < lights.size(); i++) {
		std::string target("pointLights[" + std::to_string(i) + "].");
		setUniformVec3(lights[i]->position, std::string(target + "pos").c_str());
		setUniformVec4(lights[i]->color, std::string(target + "color").c_str());
		setUniformFloat(lights[i]->radius, std::string(target + "radius").c_str());
	}
	setUniformInt(lights.size(), "lightCount");
}
