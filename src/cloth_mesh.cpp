#include "cloth_mesh.hpp"

namespace phyl{
	ClothMesh::ClothMesh(int w, int h, int l) : width(w), height(h), lod(l) {
		genMesh();
	}
	void ClothMesh::draw(){

	}
	void ClothMesh::update(const float dt){

	}
	void ClothMesh::genMesh(){
		int wVerts = width / lod;
		int hVerts = height / lod;
		vertsNumber = wVerts * hVerts;
		for(int i = 0; i < wVerts; ++i){
			for(int j = 0; j < hVerts; ++j){

			}
		}
	}
};
