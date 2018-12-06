#include "Object.h"

Object::Object() {
	model = nullptr;
}


Object::Object(ShaderHandler* shader, float TTL, glm::vec3 pos) {
	this->shader = shader;
	model = new Model();
	this->position = pos;
	this->speed = glm::vec3(0.0f, 0.0f, 0.0f);
	this->TTL = TTL;
	this->elapsedTime = 0;
}

Object::Object(ShaderHandler* shader, const char * path, bool hasUV) {
	this->shader = shader;
	model = new Model(path, hasUV);
}

Object::~Object() {
	if (model != nullptr) {
		delete model;
	}
}

void Object::loadModel(const char * path, bool hasUV) {
	if (model != nullptr) delete model;

	model = new Model(path, hasUV);
}

bool Object::particleUpdate(Object* particle, float dt)
{
	this->speed.y += 5 * dt; //Gravity
	this->change = this->speed + this->position;
	this->elapsedTime += dt;
	//std::cout << "y: " << this->speed.y << std::endl;
	//std::cout << "Elapsed: " << elapsedTime << ", dt: " << dt << elapsedTime << std::endl;
	return elapsedTime < this->TTL;
}

void Object::draw() {
	shader->setUniformMatrix4(transform.getMatrix(), "mat_world");
	model->draw();
}

Transform* Object::getTransform() const {
	return (Transform*)&transform;
}
