#include "scene.hpp"


namespace phyl {
	Scene::Scene(Options* options) {
		glm::vec3 camPosition = glm::vec3{options->getFloat("cam_pos_x", 50.0f),
									options->getFloat("cam_pos_y", 50.0f),
									options->getFloat("cam_pos_z", 50.0f)};
		glm::vec3 up = glm::vec3{options->getFloat("cam_up_x", 0.0f),
									options->getFloat("cam_up_y", 1.0f),
									options->getFloat("cam_up_z", 0.0f)};
		float fov = options->getFloat("cam_fov", 90.0f);
		camera = std::make_unique<Camera>(camPosition, up, fov, -90, 0, 0);

		int wWidth = options->getInt("win_width", 800);
		int wHeight = options->getInt("win_height", 400);
		primitives.push_back(SpherePrimitive(50));
		proj = glm::perspective(camera->getFOV(), (float)wWidth/(float)wHeight, 0.1f, 100.0f);
	}

	void Scene::draw() {
		glm::mat4 projView = proj * camera->getViewMatrix();
		for(auto &p : primitives){
			p.draw(projView, camera->getPosition());
		}
		// mmesh->draw();
	}

	void Scene::update(const float dt){
		camera->update(dt);
	}

};
