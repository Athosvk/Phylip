#include "transform.hpp"

namespace phyl {
	MTransform::MTransform() : 
	updated(false) {
		transform.translation = {0.0f, 0.0f, 0.0f};
		transform.scale = {1.0f, 1.0f, 1.0f};
	}
	
	const Matrix& MTransform::getTransformationMatrix() {
		if(!updated) updateMatrix();
		return transformMat;
	}

	void MTransform::translate(const Vector3 &translate) {
		transform.translation.x += translate.x;
		transform.translation.y += translate.y;
		transform.translation.z += translate.z;
	}

	void MTransform::rotate(const Vector3 &angle, float q){
		#warning "MTransform::rotate is not implemented!"
	}

	void MTransform::scale(const Vector3 &s){
		transform.scale.x *= s.x;
		transform.scale.y *= s.y;
		transform.scale.z *= s.z;
	}

	void MTransform::updateMatrix() {
		transformMat = Matrix{1.0f};
		transformMat = MatrixMultiply(transformMat, MatrixTranslate(transform.translation.x,transform.translation.y,transform.translation.z));
		transformMat = MatrixMultiply(transformMat, QuaternionToMatrix(transform.rotation));
		transformMat = MatrixMultiply(transformMat, MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z));

		updated = true;
	}
}
