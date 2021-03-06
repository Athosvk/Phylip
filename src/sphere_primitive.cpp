#include "sphere_primitive.hpp"

#include "defs.hpp"

#include <cassert>
#include <vector>
#include <iostream>

namespace phyl {
	SpherePrimitive::SpherePrimitive(Vector3 center, int rad, Color c) : m_radius(rad){
		m_mesh = GenMeshSphere(m_radius, 32, 32);
		m_mat = LoadMaterialDefault();
		m_transform.translate(center);
		m_mat.maps[0].color = c;
		Velocity = { 0.0, 0.0, -50 };
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
		auto move = Velocity * dt;
		m_transform.translate(Vector3 {(float)move.x(), (float)move.y(), (float)move.z()});
	}

	bool SpherePrimitive::intersection(const Eigen::Vector3d &p, Eigen::Vector3d& contactNormal, double& dist) const {
		Vector3 pos = m_transform.getTranslation();
		Eigen::Vector3d center = Eigen::Vector3d(pos.x, pos.y, pos.z);
		Eigen::Vector3d diff = p - center;
		dist = diff.norm() - m_radius - 0.3;
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
