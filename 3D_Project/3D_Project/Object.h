#pragma once
#include "Model.h"
#include "Transform.h"
#include "ShaderHandler.h"
class Object {
private:
	Model* model;

	Transform transform;

	ShaderHandler * shader;
public:
	Object();
	Object(ShaderHandler *shader);
	Object(ShaderHandler *shader, const char* path, bool hasUV);
	virtual ~Object();

	void loadModel(const char* path, bool hasUV);

	void draw();

	Transform* getTransform() const;
};

