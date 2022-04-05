#include "primitive.hpp"

#include <cassert>

namespace phyl {
	Primitive::Primitive(const std::string &fp){
		model = LoadModel(fp.c_str());
		assert(model.meshCount == 1);
		// TODO: copy material components before unloading;
		mesh = &model.meshes[0];
	}

	void Primitive::unload() {
		UnloadModel(model);
		mesh = nullptr;
	}
	void Primitive::draw() {
		DrawMesh(*mesh, model.materials[model.meshMaterial[0]], transform.getTransformationMatrix());
	}

	BoundingBox Primitive::getBBox() const {
		return GetMeshBoundingBox(*mesh);
	}

	void Primitive::update(const float dt) {
		
	}
}
