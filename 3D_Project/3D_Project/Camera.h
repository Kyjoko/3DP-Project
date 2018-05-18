#pragma once

#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <imgui.h>

class Camera {
private:
	GLFWwindow * m_window;
	glm::vec3 m_pos;
	glm::vec3 m_viewDir;
	float m_mouseSensitivity;
	float m_movementSpeed;
	
	const glm::vec3 UP;
	glm::vec2 m_oldMousePos;
	glm::vec2 m_mouseDelta;

	struct Key {
		int keyCode;
		bool pressed;
	};

	Key keys[6];

public:
	Camera();

	void update();

	void handleMouseMovement(glm::vec2 pos);
	void handleKeyInput(int key, int action);

	glm::mat4 getMatrix() const;
	glm::vec3 getPos() const;
	

};