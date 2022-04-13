#pragma once

#include "raylib.h"
#include "mtransform.hpp"

#include "Eigen/Dense"

#include <string>

namespace phyl {
	class SpherePrimitive{
		public:
			SpherePrimitive(Vector3 center, int radius = 1);

			BoundingBox getBBox() const;

			void draw();
			void update(const float dt);
			void unload();

			bool intersection(const Eigen::Vector3d &p, Eigen::Vector3d& contactNormal, double& dist) const;

			void setShader(Shader *shader);

		private:
			Mesh generate();

			Mesh m_mesh;
			Material m_mat;
			MTransform m_transform;
			int m_radius;
	};
};
