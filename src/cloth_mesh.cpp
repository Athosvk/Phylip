#include "cloth_mesh.hpp"
#include <iostream>

namespace phyl{
	ClothMesh::ClothMesh(int w, int h, double mass, int lod) : width(w), height(h), lod(lod), mass(mass) {
		genMesh();
		mat = LoadMaterialDefault();
		mat.maps[0].color.r = 225;
		mat.maps[0].color.g = 125;
		mat.maps[0].color.b = 50;
		
		// Just some test code, remove later
		for (int i = 0; i < currVelocities.rows() / 3; i++)
		{
			currVelocities[i * 3 + 1] = i * i;
		}
	}

	void ClothMesh::draw(){
		DrawMesh(mesh, mat, transform.getTransformationMatrix());
	}

	void ClothMesh::update(const float dt){
		updateMesh();
		// Will probably move to some simulation class to decouple from rendering
		integrateVelocity(dt);
	}

	ClothMesh::~ClothMesh(){
		UnloadMesh(mesh);
		UnloadMaterial(mat);
	}

	void ClothMesh::genMesh(){
		mesh = GenMeshPlane(width, height, lod, lod, true);

		int nVerts = mesh.vertexCount;

		double massPerVertex = mass/(double)nVerts;
		double invMassPerVertex = 1.0/massPerVertex;

		currPositions.resize(nVerts * 3, 1);
		currVelocities = Eigen::VectorXd::Zero(nVerts*3, 1);
		for(int i = 0; i < nVerts * 3; i++){
			currPositions[i] = mesh.vertices[i];
		}

		std::vector<Eigen::Triplet<double>> massTriplets;
		std::vector<Eigen::Triplet<double>> invMassTriplets;
		for(int i = 0; i < nVerts * 3; ++i){
			massTriplets.push_back(Eigen::Triplet<double>(i, i, massPerVertex));
			invMassTriplets.push_back(Eigen::Triplet<double>(i, i, invMassPerVertex));
		}
		massMatrix.resize(nVerts * 3, nVerts * 3);
		massMatrix.setFromTriplets(massTriplets.begin(), massTriplets.end());
		invMassMatrix.resize(nVerts * 3, nVerts * 3);
		invMassMatrix.setFromTriplets(invMassTriplets.begin(), invMassTriplets.end());
	}

	void ClothMesh::integrateVelocity(const float dt)
	{
		currPositions += dt * currVelocities;
	}

	void ClothMesh::updateMesh()
	{
		for(int i = 0; i < mesh.vertexCount * 3; i++){
			mesh.vertices[i] = currPositions[i];
		}
		// VBO index 0 are the vertex positions, see source code for other buffers
		UpdateMeshBuffer(mesh, 0, mesh.vertices, mesh.vertexCount * 3 * sizeof(float), 0);
	}

};
