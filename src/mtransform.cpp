#include "mtransform.hpp"

namespace phyl {
	MTransform::MTransform() : 
	updated(false) {
		transform.translation = {0.0f, 0.0f, 0.0f};
		transform.scale = {1.0f, 1.0f, 1.0f};
		transform.rotation = QuaternionIdentity();
		transformMat = MatrixIdentity();
	}
	
	Matrix& MTransform::getTransformationMatrix() {
		if(!updated) updateMatrix();
		return transformMat;
	}

	void MTransform::translate(const Vector3 &translate) {
		transform.translation.x += translate.x;
		transform.translation.y += translate.y;
		transform.translation.z += translate.z;
	}

	void MTransform::rotate(const Vector3 &angle, float q){
		transform.rotation = QuaternionMultiply(transform.rotation, QuaternionFromAxisAngle(angle, q));
	}

	void MTransform::scale(const Vector3 &s){
		transform.scale.x *= s.x;
		transform.scale.y *= s.y;
		transform.scale.z *= s.z;
	}

	void MTransform::updateMatrix() {
		Matrix mScale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
		Matrix mRotate = QuaternionToMatrix(transform.rotation);
		Matrix mTranslate = MatrixTranslate(transform.translation.x, transform.translation.y, transform.translation.z);
		Matrix mTransf = MatrixMultiply(MatrixMultiply(mScale, mRotate), mTranslate);
		//Matrix mTransf = MatrixMultiply(mScale, mTranslate);
		transformMat = MatrixMultiply(transformMat, mTransf);
		updated = true;
	}
}
