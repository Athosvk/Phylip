#include "scene.hpp"
#include "rlgl.h"

namespace phyl {
	Scene::Scene(Options* options) {
		camera.position = Vector3{options->getFloat("cam_pos_x", 50.0f),
									options->getFloat("cam_pos_y", 50.0f),
									options->getFloat("cam_pos_z", 50.0f)};
		camera.target = Vector3{options->getFloat("cam_dir_x", 0.0f),
									options->getFloat("cam_dir_y", 10.0f),
									options->getFloat("cam_dir_z", 0.0f)};
		camera.up = Vector3{options->getFloat("cam_up_x", 0.0f),
									options->getFloat("cam_up_y", 1.0f),
									options->getFloat("cam_up_z", 0.0f)};
		camera.fovy = options->getFloat("cam_fov", 90.0f);
		camera.projection = CAMERA_PERSPECTIVE;
		SetCameraMode(camera, CAMERA_FREE);
		SetCameraPanControl(0);

		primitives.push_back(SpherePrimitive(15));

		MTransform t;
		t.translate(Vector3{0, 20, 0});
		cloth = std::make_shared<ClothMesh>(100, 100, /*mass*/10.0, /*subdivision*/ 10);
		cloth->transformPoints(t);

		sim.setClothMesh(cloth);
		sim.setPrimitiveList(&primitives);
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
		for(auto &p : primitives){
			p.update(dt);
		}
		cloth->update(dt);
		sim.update(dt);
	}

};
