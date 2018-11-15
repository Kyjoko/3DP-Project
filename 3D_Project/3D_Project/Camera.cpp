#include "Camera.h"

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <iostream>

Camera::Camera(unsigned int screenWidth, unsigned int screenHeight, float fov) : UP(0.0f, 1.0f, 0.0f) {
	width = screenWidth;
	height = screenHeight;
	this->fov = fov;

	m_mouseSensitivity = 0.001f;
	m_movementSpeed = 3.0f;

	keys[0] = { GLFW_KEY_W, false };
	keys[1] = { GLFW_KEY_A, false };
	keys[2] = { GLFW_KEY_S, false };
	keys[3] = { GLFW_KEY_D, false };
	keys[4] = { GLFW_KEY_LEFT_SHIFT, false };
	keys[5] = { GLFW_KEY_SPACE, false };

}

void Camera::update(float dt) {
	if (keys[0].pressed) {
		transform.translate(
			transform.getPosition() + transform.getDir() * m_movementSpeed * dt);
	}
	if (keys[2].pressed) {
		transform.translate(
			transform.getPosition() - transform.getDir() * m_movementSpeed * dt);
	}
	if (keys[1].pressed) {
		transform.translate(
			transform.getPosition() + glm::cross(UP, transform.getDir()) * m_movementSpeed * dt);
	}
	if (keys[3].pressed) {
		transform.translate(
			transform.getPosition() - glm::cross(UP, transform.getDir()) * m_movementSpeed * dt);
	}
	if (keys[4].pressed) {
		transform.translate(
			transform.getPosition() - UP * m_movementSpeed * dt);
	}
	if (keys[5].pressed) {
		transform.translate(
			transform.getPosition() + UP * m_movementSpeed * dt);
	}
}

void Camera::handleMouseMovement(glm::vec2 pos) {

	m_mouseDelta = pos - m_oldMousePos;
	glm::vec3 newView = glm::mat3(glm::rotate(m_mouseDelta.y * m_mouseSensitivity, glm::cross(UP, transform.getDir()))) * transform.getDir();
	newView = glm::mat3(glm::rotate(-m_mouseDelta.x * m_mouseSensitivity, UP)) * newView;

	if (abs(newView.y) < 0.99f) {
		transform.setDir(newView);
	}
	else {
		newView = glm::mat3(glm::rotate(-m_mouseDelta.x * m_mouseSensitivity, UP)) * transform.getDir();
		transform.setDir(newView);
	}
	m_oldMousePos = pos;
}

void Camera::handleKeyInput(int key, int action) {
	for (int i = 0; i < 6; i++) {
		if (key == keys[i].keyCode && action == GLFW_PRESS) {
			keys[i].pressed = true;
		}
		else if (key == keys[i].keyCode && action == GLFW_RELEASE) {
			keys[i].pressed = false;
		}
	}
}

glm::mat4 Camera::getProjection() const {

	return glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 20.f);;
}

Transform * Camera::getTransform() const {
	return (Transform*)&transform;
}

void Camera::setFov(float fov){
	this->fov = fov;
}

float Camera::getFov() const {
	return fov;
}
