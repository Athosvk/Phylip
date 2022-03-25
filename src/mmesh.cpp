#include "mmesh.hpp"

#include <cassert>

namespace phyl {
	MMesh::MMesh(const std::string &fp){
		model = LoadModel(fp.c_str());
		assert(model.meshCount == 1);
		// TODO: copy material components before unloading;
		mesh = &model.meshes[0];
	}

	void MMesh::unload() {
		UnloadModel(model);
		mesh = nullptr;
	}
	void MMesh::draw(){
		DrawMesh(*mesh, model.materials[model.meshMaterial[0]], transform.getTransformationMatrix());
	}

	BoundingBox MMesh::getBBox() const {
		return GetMeshBoundingBox(*mesh);
	}

	void MMesh::update(const float dt) {
		
	}
}
