#include "renderer.hpp"

#include <iostream>


namespace phyl{
	Renderer::Renderer(Options *_options)
	: options{_options} {
		InitWindow(options->getInt("screen_width", 800), options->getInt("screen_height", 450), "Phylip");
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
	}

	void Renderer::run(){
		const float fps = options->getInt("target_fps", 30);
		SetTargetFPS(fps);
		while(!WindowShouldClose()){
			UpdateCamera(&camera);
			const float dt = GetFrameTime();
			std::cout << "Current dt: "<< dt << "\n";
			update(dt);
			draw();
		}
		CloseWindow();
	}

	void Renderer::draw() {
		if(currScene){
			BeginDrawing();
				ClearBackground(BLUE);
				BeginMode3D(camera);
				currScene->draw();
				DrawGrid(20.0f, 10.0f);
				EndMode3D();
			EndDrawing();
		}
	}

	void Renderer::setScene(std::shared_ptr<Scene> scene){
		currScene = scene;
	}

	void Renderer::update(const float dt) {
		if(currScene){
			currScene->update(dt);
		}
	}
}

