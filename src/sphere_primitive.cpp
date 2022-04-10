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

	bool SpherePrimitive::intersection(const Eigen::Vector3d &p, Eigen::Vector3d contactNormal, double dist) const {
		Vector3 pos = transform.getTranslation();
		Eigen::Vector3d center = Eigen::Vector3d(pos.x, pos.y, pos.z);
		Eigen::Vector3d diff = p - center;
		dist = diff.norm() - radius;
		if (dist < 0.001) {
			contactNormal = diff.normalized();
			return true;
		}
		else {
			return false;
		}
	}
}
