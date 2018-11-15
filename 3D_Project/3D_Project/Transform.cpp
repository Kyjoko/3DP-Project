#include "Transform.h"

#include <iostream>
Transform::Transform() {
	dir = glm::vec3(0.0f, 0.0f, 1.0f);
}


Transform::~Transform() {

}

void Transform::rotate(float angle, glm::vec3 axis) {
	//glm::vec3 currentPos = pos;
	//translate(glm::vec3(0, 0, 0));
	dir = glm::mat3(glm::rotate(angle, axis)) * dir;
	//translate(currentPos);
}

void Transform::translate(glm::vec3 tran) {
	pos = tran;
}

glm::mat4 Transform::getMatrix() {
	return glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Transform::getPosition() const {
	return pos;
}

glm::vec3 Transform::getDir() const {
	return dir;
}

void Transform::setDir(glm::vec3 look) {
	dir = look;
}

