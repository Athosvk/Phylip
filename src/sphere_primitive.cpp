#include "sphere_primitive.hpp"

#include "defs.hpp"

#include <cassert>
#include <vector>
#include <iostream>

namespace phyl {
	SpherePrimitive::SpherePrimitive(Vector3 center, int rad) : m_radius(rad){
		m_mesh = GenMeshSphere(m_radius, 32, 32);
		m_mat = LoadMaterialDefault();
		m_transform.translate(center);
		m_mat.maps[0].color = LIME;
	}

	void SpherePrimitive::unload() {
		UnloadMaterial(m_mat);
		UnloadMesh(m_mesh);
	}
	void SpherePrimitive::draw() {
		DrawMesh(m_mesh, m_mat, m_transform.getTransformationMatrix());
	}

	BoundingBox SpherePrimitive::getBBox() const {
		return GetMeshBoundingBox(m_mesh);
	}

	void SpherePrimitive::update(const float dt) {
		//m_transform.translate({0.0,0.0,0.1f*dt});
	}

	bool SpherePrimitive::intersection(const Eigen::Vector3d &p, Eigen::Vector3d& contactNormal, double& dist) const {
		Vector3 pos = m_transform.getTranslation();
		Eigen::Vector3d center = Eigen::Vector3d(pos.x, pos.y, pos.z);
		Eigen::Vector3d diff = p - center;
		dist = diff.norm() - m_radius - 0.1;
		if (dist < 0) {
			contactNormal = diff.normalized();
			return true;
		}
		else {
			return false;
		}
	}

	void SpherePrimitive::setShader(Shader *shader) {
		m_mat.shader = *shader;
	}
}
