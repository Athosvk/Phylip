#include "scene.hpp"
#include "rlgl.h"

namespace phyl {
	Scene::Scene(Options* options) {
		camera.position = Vector3{options->getFloat("cam_pos_x", 00.0f),
									options->getFloat("cam_pos_y", 50.0f),
									options->getFloat("cam_pos_z", 50.0f)};
		camera.target = Vector3{options->getFloat("cam_dir_x", 0.0f),
									options->getFloat("cam_dir_y", 0.0f),
									options->getFloat("cam_dir_z", 0.0f)};
		camera.up = Vector3{options->getFloat("cam_up_x", 0.0f),
									options->getFloat("cam_up_y", 1.0f),
									options->getFloat("cam_up_z", 0.0f)};
		camera.fovy = options->getFloat("cam_fov", 90.0f);
		camera.projection = CAMERA_PERSPECTIVE;
		SetCameraMode(camera, CAMERA_FREE);
		SetCameraPanControl(0);

		primitives.push_back(SpherePrimitive(20));

		MTransform t;
		t.translate(Vector3{0, 30, 0});
		cloth = std::make_shared<ClothMesh>(50, 50, /*mass*/2.0, /*subdivision*/ 30, /*hasFixed*/ false);
		//cloth = std::make_shared<ClothMesh>(50, 50, /*mass*/2.0, /*subdivision*/ 34, /*hasFixed*/ true);
		cloth->transformPoints(t);
		simulator = std::make_unique<ClothSimulator>(cloth);

		float fps = options->getInt("target_fps", 30);
		m_fixedDt = 1.0/fps;
	}

	const Camera& Scene::getCamera() const {
		return camera;
	}

	void Scene::draw() {
		rlDisableBackfaceCulling();
		cloth->draw();
		rlEnableBackfaceCulling();
		for(auto &p : primitives){
			p.draw();
		}
	}

	void Scene::update(const float dt){
		UpdateCamera(&camera);
		simulator->update(primitives, m_fixedDt);
		for(auto &p : primitives){
			p.update(dt);
		}
	}

};
