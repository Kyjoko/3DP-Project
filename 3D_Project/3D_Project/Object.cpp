#include "Object.h"

Object::Object() {
	model = nullptr;
}


Object::Object(ShaderHandler* shader) {
	this->shader = shader;
	model = new Model();
}

Object::Object(ShaderHandler* shader, const char * path, bool hasUV) {
	this->shader = shader;
	model = new Model(path, hasUV);
}

Object::~Object() {
	if(model != nullptr) delete model;
}

void Object::loadModel(const char * path, bool hasUV) {
	if (model != nullptr) delete model;

	model = new Model(path, hasUV);
}

void Object::draw() {
	shader->setUniformMatrix4(transform.getMatrix(), "mat_world");
	model->draw();
}

Transform* Object::getTransform() const {
	return (Transform*)&transform;
}
