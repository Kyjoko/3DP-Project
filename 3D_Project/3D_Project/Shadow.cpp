#include "Shadow.h"



Shadow::Shadow(ShaderHandler* shader)
{	
	this->SHADOW_WIDTH = 1024;
	this->SHADOW_HEIGHT = 1024;
	
	//when to call?
	this->depthMap(shader);
}


Shadow::~Shadow()
{
}


void Shadow::depthMap(ShaderHandler* shader)
{
	//Frame Buffer
	glGenFramebuffers(1, &this->depthMapFBO);

	//Create Texture for Depth Map
	unsigned int depthMap;
	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	shader->setUniformInt(2, "depth");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Bind buffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE); //No need for color buffer in depthmap,
	glReadBuffer(GL_NONE); //so just disable it instead.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::renderDepth(ShaderHandler* shader, Model* model)
{	
	//Configure Matrices
	shader->setUniformMatrix4(this->lightMatrix(), "mat_lightSpace");
	
	//Render to depth map
	glViewport(0, 0, this->SHADOW_WIDTH, this->SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	model->drawDepth();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 Shadow::lightMatrix()
{
	float near_plane = 1.0f, far_plane = 7.5f;

	//ProjectionMatrix
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	//ViewMatrix
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
									glm::vec3(0.0f, 0.0f, 0.0f),
									glm::vec3(0.0f, 1.0f, 0.0f));

	//PV
	glm::mat4 mat_lightSpace = lightProjection * lightView;
	return mat_lightSpace;
}
