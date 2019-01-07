#include "Terrain.h"

Terrain::Terrain(glm::vec3 pos, glm::vec2 size, float detail, const char* pathToBitmap){
	model = new Model(pos, size.x, size.y, detail, pathToBitmap);
}

Terrain::~Terrain(){

}

void Terrain::draw() {
	model->draw();
}
