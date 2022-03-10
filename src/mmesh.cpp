#include "mmesh.hpp"

#include <cassert>

namespace phyl {
	MMesh::MMesh(const std::string &fp){
		model = LoadModel(fp.c_str());
		assert(model.meshCount == 1);
		// TODO: copy material components before unloading;
		mat = LoadMaterialDefault();
		mesh = model.meshes[0];
		UnloadModelKeepMeshes(model);
	}

	void MMesh::unload() {
		UnloadMesh(mesh);
	}
	void MMesh::draw(){
		DrawMesh(mesh, mat, transform.getTransformationMatrix());
	}

	BoundingBox MMesh::getBBox() const {
		return GetMeshBoundingBox(mesh);
	}
}
