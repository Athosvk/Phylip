#include "mesh.hpp"

#include <cassert>

namespace phyl {
	MMesh::MMesh(const std::string &fp){
		Model model = LoadModel(fp.c_str());
		assert(model.meshCount == 1);
		mesh = model.meshes[0];
		Material modelMat = model.materials[model.meshMaterial[0]];
		UnloadModelKeepMeshes(model);
	}

	MMesh::~MMesh() {
		UnloadMesh(mesh);
	}

	void MMesh::draw(){
		DrawMesh(mesh, mat, transform.getTransformationMatrix());
	}
}
