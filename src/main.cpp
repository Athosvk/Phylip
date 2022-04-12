#include <iostream>

#include "options.hpp"
#include "renderer.hpp"
#include "scene.hpp"

#include "raylib.h"

#include <filesystem>

int main(int argv, char* args[]){
	phyl::Options opts;
	if(argv > 1){
		opts.loadOptions(args[1]);
	}
	phyl::Renderer rend(&opts);
	std::shared_ptr<phyl::Scene> scene = std::make_shared<phyl::Scene>(&opts);
	rend.setScene(scene);
	rend.run();
	return 0;
}
