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
			void transformPoints(MTransform &t);

			int getVertexCount() const { return mesh.vertexCount; }
			Eigen::SparseMatrix<double> getMassMatrix() const { return massMatrix; }
			Eigen::Vector3d getVertex(int i) const {return currPositions.block<3,1>(3*i, 0);};
			const Eigen::VectorXd& getPosition() { return currPositions; }
			void setPosition(const Eigen::VectorXd& newPos) { currPositions = newPos; updateMesh(); }


			Eigen::VectorXd currPositions;
			Eigen::VectorXd currVelocities;

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

			Eigen::VectorXd gravF;

			Eigen::SparseMatrix<double> massMatrix;
			Eigen::SparseMatrix<double> invMassMatrix;
		};
};
