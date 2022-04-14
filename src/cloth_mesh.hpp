#pragma once

#include "mtransform.hpp"
#include "sphere_primitive.hpp"
#include "attachment_constraint.hpp"

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
			void setShader(Shader *shader);

			uint32_t GetVertexCount() const;
			const std::vector<Edge> GetEdges() const;
			const Eigen::VectorXd& GetVertexPositions() const;
			const Eigen::VectorXd& GetVertexNormals() const;
			const Eigen::SparseMatrix<double>& GetVertexMasses() const;
			const uint32_t GetSize() const;
			void SetVertexPositions(const Eigen::VectorXd& newPositions);
			Eigen::Vector3d GetVertexPosition(int i) const;
			Eigen::Vector3d GetVertexNormal(int i) const;
			void SetVertexNormal(int i, const Eigen::Vector3d &n);
			std::vector<uint32_t> getFixedVertices() const;
			double getVertexMass() const;

		private:
			/*
			 * Generate vertices and triangles of the mesh
			 */
			void genMesh();
			/*
			 * Generate a list of edges from the plane mesh.
			 * @return the number of edges;
			 */
			int genEdges();
			/*
			 * Update the VBOs of the mesh
			 */
			void updateMesh();
			void recomputeNormals();

			int width, height, lod;
			int vertsNumber;

			double mass;
			double massPerVertex;

			Mesh mesh;
			Material mat;
			MTransform transform;
			std::vector<Edge> edges;
			std::vector<uint32_t> fixedVertices;

			Eigen::VectorXd currPositions;
			Eigen::VectorXd currNormals;
			Eigen::SparseMatrix<double> massMatrix;
			Eigen::SparseMatrix<double> invMassMatrix;
		};
};
