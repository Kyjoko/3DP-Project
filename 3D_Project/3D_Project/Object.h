#pragma once
#include "Transform.h"
#include "Shadow.h"
//#include "Model.h"
//#include "ShaderHandler.h"

class Object {
private:
	Model *model;

	Transform transform;

	ShaderHandler *shader;

	Shadow *shadow;

	float TTL; //Time To Live;
	float size;
	

public:
	Object();
	Object(ShaderHandler *shader);
	Object(ShaderHandler *shader, float TTL, glm::vec3 pos);
	Object(ShaderHandler *shader, const char* path, bool hasUV);
	virtual ~Object();

	void loadModel(const char* path, bool hasUV);

	bool particleUpdate(float dt); //Updates Billboard Particle Object Variables
	void draw();
	void drawDepth();
	void loadTex();

	glm::vec3 change;
	glm::vec3 position;
	glm::vec3 speed;

	float elapsedTime;

	Transform* getTransform() const;
};

