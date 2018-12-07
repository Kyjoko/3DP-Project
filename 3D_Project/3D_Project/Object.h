#pragma once
#include "Model.h"
#include "Transform.h"
#include "ShaderHandler.h"
class Object {
private:
	Model* model;

	Transform transform;

	ShaderHandler * shader;

	float TTL; //Time To Live;
	float size;
	

public:
	Object();
	Object(ShaderHandler *shader, float TTL, glm::vec3 pos);
	Object(ShaderHandler *shader, const char* path, bool hasUV);
	virtual ~Object();

	void loadModel(const char* path, bool hasUV);

	bool particleUpdate(float dt); //Updates Billboard Particle Object Variables
	void draw();

	glm::vec3 change;
	glm::vec3 position;
	glm::vec3 speed;

	float elapsedTime;

	Transform* getTransform() const;
};

