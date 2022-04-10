#pragma once

#include "constraint.hpp"
#include "mtransform.hpp"

#include "raylib.h"
#include "Eigen/Dense"
#include "Eigen/Sparse"

namespace phyl{
	struct Edge {
		unsigned short verts[2];
		unsigned short tris[2];
	};
	class ClothMesh {
		public:
			ClothMesh(int width, int height, double mass = 1.0, int lod = 1, bool hasFixed = false);
			~ClothMesh();

			void draw();
			void update(const float dt);

		private:
			/*
			 * Generate vertices and triangles of the mesh
			 */
			void genMesh();
			void genConstraints();
			void integrateVelocity(const float dt);
			void integratePosition(const float dt);
			void updateMesh();
			int genEdges();

			int width, height, lod;
			int vertsNumber;

			double mass;
			double gridSize;

			bool hasFixed;
			
			Mesh mesh;
			Material mat;
			MTransform transform;
			std::vector<Edge> edges;
			std::vector<Constraint> constraints;

			Eigen::VectorXd currPositions;
			Eigen::VectorXd currVelocities;
			Eigen::VectorXd gravF;

			Eigen::SparseMatrix<double> massMatrix;
			Eigen::SparseMatrix<double> invMassMatrix;
		};
};
