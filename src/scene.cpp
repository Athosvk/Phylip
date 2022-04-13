#include "scene.hpp"
#include "scene_parser.hpp"

#include "rlgl.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include <filesystem>

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

		m_shader = LoadShader("resources/shaders/lighting_vs.glsl",
				"resources/shaders/lighting_fs.glsl");
		m_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(m_shader, "viewPos");
		int ambientLoc = GetShaderLocation(m_shader, "ambient");
		float ambientCol[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		SetShaderValue(m_shader, ambientLoc, ambientCol, SHADER_UNIFORM_VEC4);
		Light lights[2] = { 0 };
		lights[0] = CreateLight(LIGHT_DIRECTIONAL, Vector3Zero(), (Vector3){ 0, -0.4, -0.6 }, YELLOW, m_shader);
		lights[1] = CreateLight(LIGHT_POINT, (Vector3){ 0, 100.3, -30.0 }, Vector3Zero(), PURPLE, m_shader);

		std::string scenePath = options->getString("scene_path", "");
		if(scenePath.empty() || !std::filesystem::exists(scenePath)){ /* Load default scene */
			primitives.push_back(SpherePrimitive({0.0f,0.0f,0.0f}, 20));
			MTransform t;
			t.translate(Vector3{0, 30, 0});
			cloth = std::make_shared<ClothMesh>(50, 50, /*mass*/6.0, /*subdivision*/ 34, /*hasFixed*/ true);
			cloth->transformPoints(t);
		} else {
			SceneParser parser(scenePath);
			parser.GetCloth(cloth);
			parser.GetPrimitives(primitives);
		}
		assert(cloth != nullptr);

		cloth->setShader(&m_shader);
		for(auto &p : primitives) {
			p.setShader(&m_shader);
		}
		simulator = std::make_unique<ClothSimulator>(options);
		simulator->setCloth(cloth);

		float fps = options->getInt("target_fps", 30);
		m_fixedDt = 1.0/fps;
	}

	Scene::~Scene(){
		UnloadShader(m_shader);
	}

	const Camera& Scene::getCamera() const {
		return camera;
	}

	void Scene::draw() {
		m_guiWrapper.draw();
		rlDisableBackfaceCulling();
		cloth->draw();
		rlEnableBackfaceCulling();
		for(auto &p : primitives){
			p.draw();
		}
	}

	void Scene::update(const float dt){
		float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
		SetShaderValue(m_shader, m_shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
		UpdateCamera(&camera);
		simulator->update(primitives, m_fixedDt);
		for(auto &p : primitives){
			p.update(dt);
		}
	}

};
