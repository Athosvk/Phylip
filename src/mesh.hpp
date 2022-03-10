#pragma once

#include "raylib.h"
#include "transform.hpp"
#include <string>

namespace phyl {
	class MMesh{
		public:
			MMesh(const std::string &fp);
			~MMesh();
			void draw();
		private:
			Mesh mesh;
			Material mat;
			MTransform transform;
	};
};
