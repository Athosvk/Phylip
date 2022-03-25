#include <iostream>

#include "options.hpp"
#include "renderer.hpp"

#include "raylib.h"

#include <filesystem>

int main(int argv, char* args[]){
	phyl::Options opts;
	phyl::Renderer rend(&opts);
	if(argv > 1){
		std::filesystem::path fp = args[1];
		if(std::filesystem::exists(fp)){
			rend.addMesh(fp);
		}
	}
	rend.run();
	return 0;
}
