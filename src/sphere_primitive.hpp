#pragma once

#include "raylib.h"
#include "mtransform.hpp"

#include "Eigen/Dense"

#include <string>

namespace phyl {
	class SpherePrimitive{
		public:
			SpherePrimitive(int radius = 1);

			BoundingBox getBBox() const;

			void draw();
			void update(const float dt);
			void unload();

			bool intersection(const Eigen::Vector3d &p, Eigen::Vector3d contactNormal, double& dist) const;

		private:
			Mesh generate();

			Model model;
			Mesh *mesh;
			MTransform transform;
			int radius;
	};
};
