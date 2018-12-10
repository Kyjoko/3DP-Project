#include "Model.h"
#include "../SOIL/src/SOIL.h" //Simple Open Image Lib

Model::Model() {

	std::array<float, 8> verticesQuad = { -0.15f, 0.15f, 
										   0.15f, 0.15f, 
										   0.15f, -0.15f,
										  -0.15f, -0.15f };

	std::array<float, 8> uvCoordQuad = { 0, 0, 
									 0, 1, 
									 1, 1,  
									 1, 0 };

	std::array<float, 6> indicesQuad = { 0,
									 2, 
									 4,  
									 0, 
									 4, 
									 6 };

	//glm::vec3 v_1 = normalize(glm::vec3(glm::vec2(verticesQuad[2], verticesQuad[3]) - glm::vec2(verticesQuad[0], verticesQuad[1]),0.0f));
	//glm::vec3 v_2 = normalize(glm::vec3(glm::vec2(verticesQuad[4], verticesQuad[5]) - glm::vec2(verticesQuad[0], verticesQuad[1]),0.0f));
	//glm::vec3 normal = cross(v_1, v_2); //Calculate normal of primitive

	TriangleVertex tmp;
	for (int i = 0; i < indicesQuad.size(); i++) {
		tmp.pos = glm::vec3(verticesQuad[indicesQuad[i]], verticesQuad[indicesQuad[i]+1], 0.0f);
		tmp.uv = glm::vec2(uvCoordQuad[indicesQuad[i]], uvCoordQuad[indicesQuad[i]+1]);
		//tmp.normal = normal;

		vertices.push_back(tmp);
	}

	//glGenTextures(1, &tex); //Generate Texture object to tex
	//glBindTexture(GL_TEXTURE_2D, tex); //Bind texture for use

	//Set Wraping
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Set Filtering
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//int width, height;
	//unsigned char* image = SOIL_load_image("../Resources/companion.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
 
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(TriangleVertex),
		&vertices[0],
		GL_DYNAMIC_DRAW);

}

float getPixel(unsigned char* image, int imageWidth, float x, float y) {
	return (float)(image[(int)(y * imageWidth + x * 4.f)] / 127.5f - 1.f);
}

glm::vec3 getNormal(unsigned char* image, int imageWidth, float x, float z) {
	if (x == 0.f)
		x = 1.f;
	if (z == 0.f)
		z = 1.f;
	float left = getPixel(image, imageWidth, x - 1.f, z);
	float right = getPixel(image, imageWidth, x + 1.f, z);
	float down = getPixel(image, imageWidth, x, z + 1.f);
	float up = getPixel(image, imageWidth, x, z - 1.f);
	return glm::normalize(glm::vec3(left - right, 1.f, down - up));
}

Model::Model(glm::vec3 pos,int width, int height, float tileSize) {

	//Load heightmap
	int imageWidth, imageHeight;
	//unsigned char* image = SOIL_load_image("../Resources/terrain-heightmap-01.png", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
	unsigned char* image = SOIL_load_image("../Resources/200px-Heightmap.png", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);

	//Make terrain
	int scale = 2;
	float y = pos.y;
	for (float tileZ = pos.x; tileZ < pos.x + height; tileZ += tileSize) {
		for (float tileX = pos.z; tileX < pos.z + width; tileX += tileSize) {
			//y += (image[(int)((tileZ - pos.z) * imageWidth + tileX - pos.x) * 4]) / 255 * 5;
			//std::cout << (image[(int)((tileZ - pos.z) * imageWidth + tileX - pos.x) * 4]) / 255 * 5 << std::endl;
			//std::cout << (int)image[(int)((tileZ - pos.z) * imageWidth + tileX - pos.x) * 4] << std::endl;
			//y = pos.y + (int)image[(int)((tileZ - pos.z) * imageWidth + tileX - pos.x) * 4] / 10;

			TriangleVertex v1{ glm::vec3(tileX, y, tileZ), glm::vec2(0, 0), glm::vec3(0, 1, 0) };
			for (int i = 0; i < 6; i++) {
				if (i == 0 || i == 3) 
					v1.pos = glm::vec3(tileX, y, tileZ);
				else if(i == 1)
					v1.pos = glm::vec3(tileX, y, tileZ + tileSize);
				else if(i == 2 || i == 4)
					v1.pos = glm::vec3(tileX + tileSize, y, tileZ + tileSize);
				else if(i == 5)
					v1.pos = glm::vec3(tileX + tileSize, y, tileZ);

				float x = (v1.pos.x - pos.x) / width * imageWidth;
				float z = (v1.pos.z - pos.z) / height * imageHeight;
				if(x == 0) std::cout << x << " : " << getPixel(image, imageWidth, x, z) << std::endl;
				v1.pos.y = pos.y + getPixel(image, imageWidth, x, z) * scale; // set height from heightmap
				v1.normal = getNormal(image, imageWidth, x, z);
				//	std::cout << getPixel(image, imageWidth, (v1.pos.x - pos.x) / width * imageWidth, (v1.pos.z - pos.z) / height * imageHeight) << std::endl;
				this->vertices.push_back(v1);
			}
			/*
			float x, z;
			TriangleVertex v1{ glm::vec3(tileX, y, tileZ), glm::vec2(0, 0), glm::vec3(0, 1, 0) };
			x = (v1.pos.x - pos.x) / width * imageWidth;
			z = (v1.pos.z - pos.z) / height * imageHeight;
			v1.pos.y = pos.y + getPixel(image, imageWidth, x, z, scale); // set height from heightmap
			v1.normal = getNormal(image, imageWidth, x, z, scale);
		//	std::cout << getPixel(image, imageWidth, (v1.pos.x - pos.x) / width * imageWidth, (v1.pos.z - pos.z) / height * imageHeight) << std::endl;
			this->vertices.push_back(v1);

			v1.pos = glm::vec3(tileX, y, tileZ + tileSize);
			x = (v1.pos.x - pos.x) / width * imageWidth;
			z = (v1.pos.z - pos.z) / height * imageHeight;
			v1.pos.y = pos.y + getPixel(image, imageWidth, x, z, scale); // set height from heightmap
			v1.normal = getNormal(image, imageWidth, x, z, scale);
		//	std::cout << getPixel(image, imageWidth, (v1.pos.x - pos.x) / width * imageWidth, (v1.pos.z - pos.z) / height * imageHeight) << std::endl;
			this->vertices.push_back(v1);

			v1.pos = glm::vec3(tileX + tileSize, y, tileZ + tileSize);
			x = (v1.pos.x - pos.x) / width * imageWidth;
			z = (v1.pos.z - pos.z) / height * imageHeight;
			v1.pos.y = pos.y + getPixel(image, imageWidth, x, z, scale); // set height from heightmap
			v1.normal = getNormal(image, imageWidth, x, z, scale);
		//	std::cout << getPixel(image, imageWidth, (v1.pos.x - pos.x) / width * imageWidth, (v1.pos.z - pos.z) / height * imageHeight) << std::endl;
			this->vertices.push_back(v1);

			v1.pos = glm::vec3(tileX, y, tileZ);
			x = (v1.pos.x - pos.x) / width * imageWidth;
			z = (v1.pos.z - pos.z) / height * imageHeight;
			v1.pos.y = pos.y + getPixel(image, imageWidth, x, z, scale); // set height from heightmap
			v1.normal = getNormal(image, imageWidth, x, z, scale);
		//	std::cout << getPixel(image, imageWidth, (v1.pos.x - pos.x) / width * imageWidth, (v1.pos.z - pos.z) / height * imageHeight) << std::endl;
			this->vertices.push_back(v1);

			v1.pos = glm::vec3(tileX + tileSize, y, tileZ + tileSize);
			x = (v1.pos.x - pos.x) / width * imageWidth;
			z = (v1.pos.z - pos.z) / height * imageHeight;
			v1.pos.y = pos.y + getPixel(image, imageWidth, x, z, scale); // set height from heightmap
			v1.normal = getNormal(image, imageWidth, x, z, scale);
		//	std::cout << getPixel(image, imageWidth, (v1.pos.x - pos.x) / width * imageWidth, (v1.pos.z - pos.z) / height * imageHeight) << std::endl;
			this->vertices.push_back(v1);
			
			v1.pos = glm::vec3(tileX + tileSize, y, tileZ);
			x = (v1.pos.x - pos.x) / width * imageWidth;
			z = (v1.pos.z - pos.z) / height * imageHeight;
			v1.pos.y = pos.y + getPixel(image, imageWidth, x, z, scale); // set height from heightmap
			v1.normal = getNormal(image, imageWidth, x, z, scale);
		//	std::cout << getPixel(image, imageWidth, (v1.pos.x - pos.x) / width * imageWidth, (v1.pos.z - pos.z) / height * imageHeight) << std::endl;
			this->vertices.push_back(v1);
			*/
		}
	}


	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(TriangleVertex),
		&vertices[0],
		GL_STATIC_DRAW);
}

Model::Model(const char* path, bool hasUV) {
	if (!load(path, hasUV)) {
		std::cout << "Could not load model from " << path << std::endl;
	}

	//glGenTextures(1, &tex); //Generate Texture object to tex
	//glBindTexture(GL_TEXTURE_2D, tex); //Bind texture for use

	////Set Wraping
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	////Set Filtering
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Image pixels
	/*float check[] = {
		0.2f, 0.2f, 0.2f,	1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,	0.2f, 0.2f, 0.2f
	};*/

	//Inladdning av bild fr�n fil till textur
	//int width, height;
	//unsigned char* image = SOIL_load_image("../Resources/companion.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	//
	//if (hasUV != true)
	//{
	//	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, check);
	//}
	//else
	//{
	//	//Load image
	//	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//}

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(TriangleVertex),
		&vertices[0],
		GL_DYNAMIC_DRAW);
}


Model::~Model()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Model::load(const char* path, bool has_uv) {

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vert;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* objFile = fopen(path, "r");
	if (objFile == NULL) return false;
	while (true) {
		char lineStart[256];
		int res = fscanf(objFile, "%s", lineStart);
		if (res == EOF) break;

		if (strcmp(lineStart, "v") == 0) { //pos
			glm::vec3 v;
			fscanf(objFile, "%f %f %f\n", &v.x, &v.y, &v.z);
			temp_vert.push_back(v);
		}
		else if (strcmp(lineStart, "vt") == 0) { //uvs
			glm::vec2 u;
			fscanf(objFile, "%f %f\n", &u.x, &u.y);
			temp_uvs.push_back(u);
		}
		else if (strcmp(lineStart, "vn") == 0) { //normals
			glm::vec3 n;
			fscanf(objFile, "%f %f %f\n", &n.x, &n.y, &n.z);
			temp_normals.push_back(n);
		}
		else if (strcmp(lineStart, "f") == 0) { //faces
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			if (has_uv) {

				int match = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0],
					&uvIndex[0],
					&normalIndex[0],
					&vertexIndex[1],
					&uvIndex[1],
					&normalIndex[1],
					&vertexIndex[2],
					&uvIndex[2],
					&normalIndex[2]
				);
				if (match != 9) return false;

				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);

			}
			else
			{
				int match = fscanf(objFile, "%d//%d %d//%d %d//%d\n",
					&vertexIndex[0],
					&normalIndex[0],
					&vertexIndex[1],
					&normalIndex[1],
					&vertexIndex[2],
					&normalIndex[2]
				);
				if (match != 6) return false;

			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		TriangleVertex tmp;
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vert[vertexIndex - 1];
		tmp.pos = vertex;

		if (uvIndices.size() > i) {
			unsigned int uvIndex = uvIndices[i];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			tmp.uv = uv;
		}
		if (normalIndices.size() > i) {
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 normal = temp_normals[normalIndex - 1];
			tmp.normal = normal;
		}

		this->vertices.push_back(tmp);
	}
}

GLuint* Model::getTex()
{
	return tex;
}

void Model::draw() const {
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (const void*) sizeof(glm::vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (const void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Model::drawParticle() const {
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (const void*) sizeof(glm::vec3));

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}