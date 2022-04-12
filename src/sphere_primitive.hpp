#pragma once

#include "raylib.h"
#include "mtransform.hpp"

#include "Eigen/Dense"

#include <string>

namespace phyl {
	class SpherePrimitive{
		public:
			SpherePrimitive(Vector3 center, int radius = 1);

			void draw();
			void update(const float dt);
			void unload();

			bool intersection(const Eigen::Vector3d &p, Eigen::Vector3d& contactNormal, double& dist) const;

		private:
			Mesh generate();

			int radius;
			Vector3 center;
	};
};
