#include "Model.h"
#include "../SOIL/src/SOIL.h" //Simple Open Image Lib


Model::Model() {

	std::array<float, 8> verticesQuad = { -0.5f, 0.5f, 
										   0.5f, 0.5f, 
										   0.5f, -0.5f,
										  -0.5f, -0.5f };

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

	glm::vec3 v_1 = normalize(glm::vec3(glm::vec2(verticesQuad[2], verticesQuad[3]) - glm::vec2(verticesQuad[0], verticesQuad[1]),0.0f));
	glm::vec3 v_2 = normalize(glm::vec3(glm::vec2(verticesQuad[4], verticesQuad[5]) - glm::vec2(verticesQuad[0], verticesQuad[1]),0.0f));
	glm::vec3 normal = cross(v_1, v_2); //Calculate normal of primitive

	TriangleVertex tmp;
	for (int i = 0; i < indicesQuad.size(); i++) {
		tmp.pos = glm::vec3(verticesQuad[indicesQuad[i]], verticesQuad[indicesQuad[i]+1], 0.0f);
		tmp.uv = glm::vec2(uvCoordQuad[indicesQuad[i]], uvCoordQuad[indicesQuad[i]+1]);
		tmp.normal = normal;

		vertices.push_back(tmp);
	}

	glGenTextures(1, &tex); //Generate Texture object to tex
	glBindTexture(GL_TEXTURE_2D, tex); //Bind texture for use

	//Set Wraping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Set Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height;
	unsigned char* image = SOIL_load_image("../Resources/companion.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
 
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(TriangleVertex),
		&vertices[0],
		GL_DYNAMIC_DRAW);

}

Model::Model(const char* path, bool hasUV) {
	if (!load(path, hasUV)) {
		std::cout << "Could not load model from " << path << std::endl;
	}

	glGenTextures(1, &tex); //Generate Texture object to tex
	glBindTexture(GL_TEXTURE_2D, tex); //Bind texture for use

	//Set Wraping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Set Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Image pixels
	float check[] = {
		0.2f, 0.2f, 0.2f,	1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,	0.2f, 0.2f, 0.2f
	};

	//Inladdning av bild från fil till textur
	int width, height;
	unsigned char* image = SOIL_load_image("../Resources/companion.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	
	if (hasUV != true)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, check);
	}
	else
	{
		//Load image
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}

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
	glBindTexture(GL_TEXTURE_2D, tex);
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