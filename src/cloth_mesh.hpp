#pragma once

#include "mtransform.hpp"

#include "raylib.h"
#include "Eigen/Dense"
#include "Eigen/Sparse"

namespace phyl{
	struct Edge
	{
		unsigned int VertexStart, VertexEnd;
		unsigned int TriStart, TriEnd;
	};
	class ClothMesh {
		public:
			ClothMesh(int width, int height, double mass = 1.0, int lod = 1);
			~ClothMesh();
			void draw();
			void update(const float dt);

			int GetVertexCount() const;
			const std::vector<Edge> GetEdges() const;
			const Eigen::VectorXd& GetVertexPositions() const;
			const Eigen::VectorXd& GetVertexVelocities() const;
			const Eigen::SparseMatrix<double>& GetVertexMasses() const;
			const int GetSize() const;
		private:
			/*
			 * Generate vertices and triangles of the mesh
			 */
			void genMesh();
			void integrateVelocity(const float dt);
			void updateMesh();

			int width, height, lod;

			double mass;
			
			Mesh mesh;
			Material mat;
			MTransform transform;

			Eigen::VectorXd currPositions;
			Eigen::VectorXd currVelocities;
			std::vector<Edge> edges;
			Eigen::SparseMatrix<double> massMatrix;
			Eigen::SparseMatrix<double> invMassMatrix;
		};
};
