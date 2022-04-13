#include "renderer.hpp"

namespace phyl{
	Renderer::Renderer(Options *_options)
	: options{_options} {
		InitWindow(options->getInt("screen_width", 800), options->getInt("screen_height", 450), "Phylip");
	}

	void Renderer::run(){
		const float fps = options->getInt("target_fps", 30);
		SetTargetFPS(fps);
		while(!WindowShouldClose()){
			const float dt = GetFrameTime();
			update(dt);
			draw();
		}
		CloseWindow();
	}

	void Renderer::draw() {
		if(currScene){
			BeginDrawing();
				ClearBackground(WHITE);
				BeginMode3D(currScene->getCamera());
				currScene->draw();
				//DrawGrid(10.0f, 10.0f);
				EndMode3D();
				currScene->drawUI();
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

