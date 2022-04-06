#include "sphere_primitive.hpp"

#include "defs.hpp"

#include <cassert>
#include <vector>

namespace phyl {
	SpherePrimitive::SpherePrimitive(int rad) : radius(rad){
		model = LoadModelFromMesh(GenMeshSphere(radius, 32, 32));
		mesh = &model.meshes[0];
	}

	void SpherePrimitive::unload() {
		UnloadModel(model);
		mesh = nullptr;
	}
	void SpherePrimitive::draw() {
		DrawMesh(*mesh, model.materials[model.meshMaterial[0]], transform.getTransformationMatrix());
	}

	BoundingBox SpherePrimitive::getBBox() const {
		return GetMeshBoundingBox(*mesh);
	}

	void SpherePrimitive::update(const float dt) {
		
	}
}
