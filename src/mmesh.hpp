#pragma once

#include "raylib.h"
#include "mtransform.hpp"
#include <string>

namespace phyl {
	class MMesh{
		public:

			MMesh(const std::string &fp);

			BoundingBox getBBox() const;

			void draw();
			void unload();

		private:
			Model model;
			Mesh mesh;
			Material mat;
			MTransform transform;
	};
};
