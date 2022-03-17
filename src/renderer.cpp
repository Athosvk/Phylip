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
		meshes.push_back(MMesh("/home/lorenzo/Code/Lorenzo/Tracey/scenes/obj/cat.obj"));
	}

	void Renderer::run(){
		SetTargetFPS(60);
		while(!WindowShouldClose()){
			UpdateCamera(&camera);
			draw();
		}

		CloseWindow();
	}

	void Renderer::draw() {
		BeginDrawing();
			ClearBackground(BLUE);
			BeginMode3D(camera);
				for(auto &m : meshes){
					m.draw();
				}
				DrawGrid(20.0f, 10.0f);
			EndMode3D();
		EndDrawing();
	}
}
