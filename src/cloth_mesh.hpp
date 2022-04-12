#pragma once

#include "mtransform.hpp"
#include "sphere_primitive.hpp"

#include "raylib.h"
#include "Eigen/Dense"
#include "Eigen/Sparse"

namespace phyl{
	struct Edge {
		unsigned short VertexStart;
		unsigned short VertexEnd;
		unsigned short tris[2];
	};
	class ClothMesh {
		public:
			ClothMesh(int width, int height, double mass = 1.0, int lod = 1, bool hasFixed = false);
			~ClothMesh();

			void draw();
			void update(const float dt);
			void transformPoints(MTransform &t);

			uint32_t GetVertexCount() const;
			const std::vector<Edge> GetEdges() const;
			const Eigen::VectorXd& GetVertexPositions() const;
			void SetVertexVelocities(const Eigen::VectorXd& newPositions);
			const Eigen::SparseMatrix<double>& GetVertexMasses() const;
			const uint32_t GetSize() const;
			void SetVertexPositions(const Eigen::VectorXd& newPositions);
			Eigen::Vector3d GetVertex(int i) const;

		private:
			/*
			 * Generate vertices and triangles of the mesh
			 */
			void genMesh();
			void updateMesh();
			int genEdges();

			int width, height, lod;
			int vertsNumber;

			double mass;
			bool hasFixed;

			Mesh mesh;
			Material mat;
			MTransform transform;
			std::vector<Edge> edges;

			Eigen::VectorXd currPositions;
			Eigen::VectorXd currVelocities;
			Eigen::SparseMatrix<double> massMatrix;
			Eigen::SparseMatrix<double> invMassMatrix;
		};
};
