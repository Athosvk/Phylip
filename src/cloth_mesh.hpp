#pragma once

#include "mtransform.hpp"

#include "raylib.h"
#include "Eigen/Dense"
#include "Eigen/Sparse"

namespace phyl{
	class ClothMesh {
		public:
			ClothMesh(int width, int height, double mass = 1.0, int lod = 1);
			~ClothMesh();
			void draw();
			void update(const float dt);


		private:
			/*
			 * Generate vertices and triangles of the mesh
			 */
			void genMesh();

			int width, height, lod;
			int vertsNumber;

			double mass;
			
			Mesh mesh;
			Material mat;
			MTransform transform;

			Eigen::VectorXd currPositions;
			Eigen::VectorXd currVelocities;
			Eigen::SparseMatrix<double> massMatrix;
			Eigen::SparseMatrix<double> invMassMatrix;
		};
};
