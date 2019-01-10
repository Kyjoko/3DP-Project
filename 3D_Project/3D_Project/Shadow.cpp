#include "Shadow.h"

Shadow::Shadow(ShaderHandler* shader)
{	
	this->SHADOW_WIDTH = 1024;
	this->SHADOW_HEIGHT = 1024;

	depthMapFBO = 0;
	depthTex = 0;

	initDepth(shader);
}

Shadow::~Shadow()
{
}
	
void Shadow::renderDepth(ShaderHandler* shader, std::vector<Object*> listA, std::vector<Terrain*> listB)
{	
	shader->setUniformMatrix4(lightMatrix(), "mat_lightSpace");
	
	//Render to depth map
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < listB.size(); i++)
	{
		listB[i]->draw();
	}
	for (int i = 0; i < listA.size(); i++)
	{
		listA[i]->draw();
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::initDepth(ShaderHandler * shader)
{
	//Create Texture for Depth Map
	glGenTextures(1, &depthTex);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	shader->setUniformInt(0, "depth");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Frame Buffer
	glGenFramebuffers(1, &depthMapFBO);

	//Bind buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
	glDrawBuffer(GL_NONE); //No need for color buffer in depthmap,
	glReadBuffer(GL_NONE); //so just disable it instead.

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "<-- Buffer Complete: " << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

glm::mat4 Shadow::lightMatrix()
{
	float near_plane = 0.0f, far_plane = 15.0f;

	//ProjectionMatrix
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	//ViewMatrix
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 3.0f, 0.0f),
									glm::vec3(0.0f, 0.0f, 0.0f),
									glm::vec3(0.0f, 1.0f, 0.0f));

	//PV
	glm::mat4 mat_lightSpace = lightProjection * lightView;
	return mat_lightSpace;
}
