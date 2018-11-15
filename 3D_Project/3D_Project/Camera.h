#pragma once
#include "Transform.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\transform.hpp>


#include <imgui.h>

class Camera {
private:
	GLFWwindow * m_window;
	Transform transform;
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

	unsigned int width, height;
	float fov;

public:
	Camera(unsigned int screenWidth, unsigned int screenHeight, float fov);

	void update(float dt);

	void handleMouseMovement(glm::vec2 pos);
	void handleKeyInput(int key, int action);

	glm::mat4 getProjection() const;
	Transform* getTransform() const;
	
	void setFov(float fov);
	float getFov() const;
};