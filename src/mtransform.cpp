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
		updated = false;
	}

	void MTransform::rotate(const Vector3 &angle, float q){
		transform.rotation = QuaternionMultiply(transform.rotation, QuaternionFromAxisAngle(angle, DEG2RAD*q));
		updated = false;
	}

	void MTransform::scale(const Vector3 &s){
		transform.scale.x *= s.x;
		transform.scale.y *= s.y;
		transform.scale.z *= s.z;
		updated = false;
	}

	Eigen::Matrix4d MTransform::getEigenTransformationMatrix() {
		auto m = getTransformationMatrix();
		Eigen::MatrixXd eigenM = Eigen::MatrixXd::Identity(4,4);
		eigenM << 	m.m0, m.m4, m.m8, m.m12,
					m.m1, m.m5, m.m9, m.m13,
					m.m2, m.m6, m.m10, m.m14,
					m.m3, m.m7, m.m11, m.m15;
		return eigenM;
	}

	void MTransform::updateMatrix() {
		Matrix mScale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
		Matrix mRotate = QuaternionToMatrix(transform.rotation);
		Matrix mTranslate = MatrixTranslate(transform.translation.x, transform.translation.y, transform.translation.z);
		transformMat = MatrixMultiply(MatrixMultiply(mScale, mRotate), mTranslate);
		updated = true;
	}
}
