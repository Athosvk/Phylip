#include "sphere_primitive.hpp"

#include "defs.hpp"

#include <cassert>
#include <vector>

namespace phyl {
	SpherePrimitive::SpherePrimitive(Vector3 center, int rad) : radius(rad), center(center) {}

	void SpherePrimitive::unload() {}

	void SpherePrimitive::draw() {
		DrawSphere(center, radius, WHITE);
	}

	void SpherePrimitive::update(const float dt) {
		//center = Vector3{center.x + 0.0f, center.y + 1.0f * dt, center.z + 0.0f};
	}

	bool SpherePrimitive::intersection(const Eigen::Vector3d &p, Eigen::Vector3d& contactNormal, double& dist) const {
		Eigen::Vector3d centerEigen = Eigen::Vector3d(center.x, center.y, center.z);
		Eigen::Vector3d diff = p - centerEigen;
		dist = diff.norm() - radius - 0.1;
		if (dist < 0) {
			contactNormal = diff.normalized();
			return true;
		}
		else {
			return false;
		}
	}
}
