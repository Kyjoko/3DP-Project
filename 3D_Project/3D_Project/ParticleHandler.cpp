#include "ParticleHandler.h"

ParticleHandler::ParticleHandler()
{

}

ParticleHandler::~ParticleHandler()
{

}

void ParticleHandler::cleanup()
{

}

void ParticleHandler::prepare()
{

}

bool ParticleHandler::update(float dt)
{
	this->speed.y += 9.82 * dt;
	glm::vec3 difference = glm::vec3(this->speed);
	difference *= dt;
	elapsedTime += dt;
	return elapsedTime > this->TTL;
}
