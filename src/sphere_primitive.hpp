#pragma once

#include "raylib.h"
#include "mtransform.hpp"
#include <string>

namespace phyl {
	class SpherePrimitive{
		public:
			SpherePrimitive(int radius = 1);

			BoundingBox getBBox() const;

			void draw() ;
			void update(const float dt);
			void unload();

		private:
			Mesh generate();

			Model model;
			Mesh *mesh;
			MTransform transform;
			int radius;
	};
};
