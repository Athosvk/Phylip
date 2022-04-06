#include <iostream>

#include "options.hpp"
#include "renderer.hpp"
#include "scene.hpp"

#include <filesystem>

int main(int argv, char* args[]){
	phyl::Options opts;
	phyl::Renderer rend(&opts);
	std::shared_ptr<phyl::Scene> scene = std::make_shared<phyl::Scene>(&opts);
	if(argv > 1){
		std::filesystem::path fp = args[1];
		if(std::filesystem::exists(fp)){
		}
	}
	rend.setScene(scene);
	rend.run();
	return 0;
}
