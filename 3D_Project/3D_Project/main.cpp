#define _CRT_SECURE_NO_DEPRECATE
#define GLM_FORCE_CTOR_INIT //Skillnad? <-- Noll aning

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <vector>

#include "Display.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

int main(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	Display* display = new Display(WIDTH, HEIGHT, "Best Gaem");
	delete display;
	
	return 0;
}