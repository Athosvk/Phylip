#pragma once

#include "Eigen/Core"

namespace phyl{
	class ClothMesh {
		public:
			ClothMesh(int width, int height, int lod = 1);
			void draw();
			void update(const float dt);

		private:
			/*
			 * Generate vertices and triangles of the mesh
			 */
			void genMesh();

			int width, height, lod;
			int vertsNumber;
			
			Eigen::VectorXd currVelocities;
			Eigen::VectorXd currPositions;
	};
};
