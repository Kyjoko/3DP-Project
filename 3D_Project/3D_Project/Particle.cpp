#include "Particle.h"

unsigned int UVBufferID;
unsigned int VertexBufferID;

Particle::Particle()
{
}

Particle::Particle(ShaderHandler* shaderHandler)
{
	this->shaderHandler = shaderHandler;

	this->setTexture();

	this->vertices = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };
	this->uvCoord = { 0, 1, 0, 0, 1, 1, 1, 0 };
}

Particle::~Particle()
{
}

void Particle::setTexture()
{
	//Skapande av textur, inladdning av bild från fil
	int width, height;
	unsigned char* image = SOIL_load_image("../Resources/companions.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//Buffers
	glGenBuffers(1, &VertexBufferID);
	glGenBuffers(1, &UVBufferID);

	//Shaders
	shaderHandler->addShader(new Shader("ParticleVertexShader.vs", "ParticleFragmentShader.fs"), "Particles");
	shaderHandler->use("Particles");

	//Uniforms
	shaderHandler->setUniformInt(0, "textureSampler");
}

void Particle::drawParticle (int x, int y, int size) {

	//rita skiten
}


void Particle::cleanup()
{
	// Delete buffers
	glDeleteBuffers(1, &VertexBufferID);
	glDeleteBuffers(1, &UVBufferID);

	// Delete texture
	//glDeleteTextures(1, &Text2DTextureID);
}

void Particle::prepare() {

	//gör skit med buffers
}

bool Particle::update(float dt)
{
	float tpf = 1 / dt; //Tid per frame

	this->speed.y += 9.82 * tpf;
	glm::vec3 difference = glm::vec3(this->speed);
	difference *= tpf;
	elapsedTime += tpf;
	return elapsedTime > this->TTL;
}
