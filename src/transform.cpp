#include "transform.hpp"

namespace phyl {
	Transform::Transform() : 
	updated(false) {
		translation = {0.0f, 0.0f, 0.0f};
		scaling = {1.0f, 1.0f, 1.0f};
	}
	
	const glm::mat4& Transform::getTransformationMatrix() {
		if(!updated) updateMatrix();
		return transformMat;
	}

	void Transform::translate(const glm::vec3 &translate) {
		translation.x += translate.x;
		translation.y += translate.y;
		translation.z += translate.z;
	}

	void Transform::rotate(const glm::vec3 &angle, float q){
		rotation = rotation * glm::angleAxis(q, angle);
	}

	void Transform::scale(const glm::vec3 &s){
		scaling.x *= s.x;
		scaling.y *= s.y;
		scaling.z *= s.z;
	}

	void Transform::updateMatrix() {
		transformMat = glm::mat4{1.0f};
		transformMat = glm::translate(transformMat, translation);
		transformMat = transformMat * glm::toMat4(rotation);
		transformMat = glm::scale(transformMat, scaling);

		updated = true;
	}
}
