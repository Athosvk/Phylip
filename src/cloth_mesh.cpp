#include "cloth_mesh.hpp"
#include <iostream>

namespace phyl{
	ClothMesh::ClothMesh(int w, int h, double mass, int lod) : width(w), height(h), lod(lod), mass(mass) {
		genMesh();
		mat = LoadMaterialDefault();
		mat.maps[0].color.r = 225;
		mat.maps[0].color.g = 125;
		mat.maps[0].color.b = 50;
	}

	void ClothMesh::draw(){
		DrawMesh(*mesh, mat, transform.getTransformationMatrix());
	}

	void ClothMesh::update(const float dt){

	}
	void ClothMesh::genMesh(){
		Mesh m = {0};
		int nVerts = width * height;
		int nFaces = (width-1) * (height-1);
		int numIdxs = nFaces * 6;

		m.vertexCount = nVerts;
		m.triangleCount = nFaces;

		double massPerVertex = mass/(double)nVerts;
		double invMassPerVertex = 1.0/massPerVertex;

		m.vertices = (float*) MemAlloc(nVerts * 3 *sizeof(float));
		m.normals = (float*) MemAlloc(nVerts * 3 *sizeof(float));
		m.colors = (unsigned char*) MemAlloc(nVerts * 3 *sizeof(unsigned char));
		//m.texcoords = (float*) MemAlloc(nVerts * 3 *sizeof(float));

		currPositions.resize(nVerts * 3, 1);
		currVelocities = Eigen::VectorXd::Zero(nVerts*3, 1);

		for(int h = 0; h < height; ++h){
			for(int w = 0; w < width; ++w){
				int idx = h * width + w;
				currPositions[3*idx+0] = w;
				currPositions[3*idx+1] = h;
				currPositions[3*idx+2] = 0;

				m.vertices[3*idx+0] = w;
				m.vertices[3*idx+1] = h;
				m.vertices[3*idx+2] = 0;

				m.colors[3*idx+0] = color[0];
				m.colors[3*idx+1] = color[1];
				m.colors[3*idx+2] = color[2];
				std::cout << idx <<"\n";
			}
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

		m.indices = (unsigned short*)MemAlloc(nFaces * 3 * sizeof(unsigned short));
		
		int offset = 0;
		for(int i = 0; i < numIdxs; ++i){
			unsigned int cornerIdx = i/6 + offset;
			if((cornerIdx + 1)%width == 0){
				offset++;
				cornerIdx++; 
				// First triangle
				m.indices[i] = (unsigned int)cornerIdx;
				i++;
				m.indices[i] = (unsigned int)cornerIdx + width;
				i++;
				m.indices[i] = (unsigned int)cornerIdx + width + 1;
				i++;

				// Second triangle
				m.indices[i] = (unsigned int)cornerIdx;
				i++;
				m.indices[i] = (unsigned int)cornerIdx + width + 1;
				i++;
				m.indices[i] = (unsigned int)cornerIdx + 1;
			}
		}
		UploadMesh(&m, false);
		model = LoadModelFromMesh(m);
		mesh = &model.meshes[0];
	}

};
