#pragma once

#include "raylib.h"
#include "options.hpp"
#include "scene.hpp"

#include <vector>
#include <filesystem>

namespace phyl{
	class Renderer{
		public:
			Renderer(Options *option);

			void run();
			void addMesh(const std::filesystem::path &fp);
			void setScene(std::shared_ptr<Scene> scene);

		private:
			Options *options;
			std::shared_ptr<Scene> currScene;

			void draw();
			void update(const float dt);
	};
}
