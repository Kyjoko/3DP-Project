#pragma once
#include "Shader.h"
#include "Camera.h"
#include <vector>
#include <map>

struct PointLight {
	glm::vec3 position;
	glm::vec4 color;
	float radius;
};

class ShaderHandler {
private:
	std::map<std::string, Shader*> shaders;
	std::string active;

	Camera* cam;
	std::vector<PointLight> lights;

public:
	ShaderHandler(Camera* cam);
	virtual ~ShaderHandler();

	void updateView();

	void setUniformMatrix4(glm::mat4 mat, const char* uniformName);
	void setUniformVec4(glm::vec4 vec, const char* uniformName);
	void setUniformVec3(glm::vec3 vec, const char* uniformName);
	void setUniformFloat(float f, const char* uniformName);
	void setUniformInt(int i, const char* uniformName);

	std::string getActive() const;
	Shader* getActiveShader() const;
	void use(std::string shaderName);

	void addShader(Shader* shader, std::string name);
	void addLight(PointLight light);
	void updateLights();
};

