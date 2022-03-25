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
			void update(const float dt);
			void unload();

		private:
			Model model;
			Mesh *mesh;
			MTransform transform;
	};
};
