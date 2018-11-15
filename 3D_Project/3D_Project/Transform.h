#pragma once
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\transform.hpp>


class Transform {
private:
	//glm::mat4 matrix;

	glm::vec3 pos;
	glm::vec3 dir;

public:
	Transform();
	virtual ~Transform();

	void rotate(float angle, glm::vec3 axis);
	void translate(glm::vec3 tran);

	glm::mat4 getMatrix(); //const?

	glm::vec3 getPosition() const;
	glm::vec3 getDir() const;

	void setDir(glm::vec3 look);
};

