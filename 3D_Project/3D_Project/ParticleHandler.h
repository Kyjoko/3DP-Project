#pragma once

#include <array>
#include "Shaderhandler.h"
#include "../SOIL/src/SOIL.h" //Simple Open Image Lib

/*
Particle klassen ska snabbt rita en quad p� korrekt
position, f�r att skapa en Particle med hj�lp av Billboards
*/

class ParticleHandler
{
public:
	ParticleHandler();
	virtual ~ParticleHandler();

	void cleanup();
	void prepare();

	bool update(float dt);

private:
	glm::vec3 position;
	glm::vec3 speed;

	float TTL; //Time To Live;
	float size;
	float elapsedTime;
};
