#pragma once

#include "mmesh.hpp"
#include "options.hpp"

#include "raylib.h"

#include <vector>
#include <filesystem>

namespace phyl{
	class Renderer{
		public:
			Renderer(Options *option);

			void run();
			void addMesh(const std::filesystem::path &fp);
			void update(const float dt);
		private:
			Options *options;
			Camera camera;
			std::vector<MMesh> meshes;

			void draw();
	};
}
