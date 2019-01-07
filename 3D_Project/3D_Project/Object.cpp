#include "Object.h"

Object::Object() {
	model = nullptr;
}

Object::Object(ShaderHandler* shader) {
	this->shader = shader;
	shadow = new Shadow(shader);
	model = new Model();
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
	if (shadow != nullptr) {
		delete shadow;
	}
}

void Object::loadModel(const char * path, bool hasUV) {
	if (model != nullptr) delete model;

	model = new Model(path, hasUV);
}

bool Object::particleUpdate(float dt)
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

void Object::drawDepth()
{
	this->shadow->renderDepth(shader, model);
}

Transform* Object::getTransform() const {
	return (Transform*)&transform;
}

void Object::loadTex()
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(2, model->getTex()); //Generate Texture object to tex
	glBindTexture(GL_TEXTURE_2D, model->getTex()[0]); //Bind texture for use

	int width, height;
	unsigned char* image = SOIL_load_image("../Resources/companion.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	shader->setUniformInt(0, "tex");

	//Set Wraping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Set Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, model->getTex()[1]); //Bind texture for use
	image = SOIL_load_image("../Resources/snow.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	shader->setUniformInt(1, "particle");

	//Set Wraping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Set Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

