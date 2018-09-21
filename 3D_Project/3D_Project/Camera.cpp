#include "Camera.h"
#include <gtx\transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <iostream>

Camera::Camera() : UP(0.0f, 1.0f, 0.0f) {
	m_pos = glm::vec3(0, 0, -2);
	m_viewDir = glm::vec3(0, 0, 1);
	m_mouseSensitivity = 0.001f;
	m_movementSpeed = 0.05f;

	keys[0] = { GLFW_KEY_W, false };
	keys[1] = { GLFW_KEY_A, false };
	keys[2] = { GLFW_KEY_S, false };
	keys[3] = { GLFW_KEY_D, false };
	keys[4] = { GLFW_KEY_LEFT_SHIFT, false };
	keys[5] = { GLFW_KEY_SPACE, false };
}

void Camera::update() {
	if (keys[0].pressed) {
		m_pos += m_viewDir * m_movementSpeed;
	}
	if (keys[2].pressed) {
		m_pos -= m_viewDir * m_movementSpeed;
	}
	if (keys[1].pressed) {
		m_pos += glm::cross(UP, m_viewDir) * m_movementSpeed;
	}
	if (keys[3].pressed) {
		m_pos -= glm::cross(UP, m_viewDir) * m_movementSpeed;
	}
	if (keys[4].pressed) {
		m_pos -= UP * m_movementSpeed;
	}
	if (keys[5].pressed) {
		m_pos += UP * m_movementSpeed;
	}

}

void Camera::handleMouseMovement(glm::vec2 pos) {

	m_mouseDelta = pos - m_oldMousePos;

	glm::vec3 newView = glm::mat3(glm::rotate(m_mouseDelta.y * m_mouseSensitivity, glm::cross(UP, m_viewDir))) * m_viewDir;
	newView = glm::mat3(glm::rotate(-m_mouseDelta.x * m_mouseSensitivity, UP)) * newView;

	if (abs(newView.y) < 0.999f) {
		//m_viewDir = glm::mat3(glm::rotate(-m_mouseDelta.x * m_mouseSensitivity, UP)) * m_viewDir;
		//m_viewDir = glm::mat3(glm::rotate(m_mouseDelta.y * m_mouseSensitivity, glm::cross(UP, m_viewDir))) * m_viewDir;

		m_viewDir = newView;
	}
	else {
	
		newView = glm::mat3(glm::rotate(-m_mouseDelta.x * m_mouseSensitivity, UP)) * m_viewDir;
		m_viewDir = newView;
	}

	std::cout << m_viewDir.y << std::endl;

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

glm::mat4 Camera::getMatrix() const{ 
	return glm::lookAt(m_pos, m_pos + m_viewDir, UP);
}

glm::vec3 Camera::getPos() const{
	return m_pos;
}
