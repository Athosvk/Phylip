#pragma once

#include "mmesh.hpp"
#include "options.hpp"

#include "raylib.h"

#include <vector>

namespace phyl{
	class Renderer{
		public:
			Renderer(Options *option);

			void run();
		private:
			Options *options;
			Camera camera;
			std::vector<MMesh> meshes;

			void draw();
	};
}
