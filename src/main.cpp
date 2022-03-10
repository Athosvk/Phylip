#include <iostream>

#include "options.hpp"
#include "renderer.hpp"

#include "raylib.h"

int main(int argv, char* args[]){
	phyl::Options opts;
	phyl::Renderer rend(&opts);
	rend.run();
	return 0;
}
