#include "scene.hpp"

namespace phyl {
	Scene::Scene() {}

	void Scene::draw() {
		for(auto &p : primitives){
			p.draw();
		}
		// mmesh->draw();
	}

	void Scene::update(const float dt){
	}

	void Scene::addPrimitive(const std::string fp){
		primitives.push_back(Primitive(fp));
	}
};
