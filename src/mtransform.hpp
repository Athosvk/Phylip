#pragma once

#include "raylib.h"
#include "raymath.h"

#include "Eigen/Dense"

namespace phyl {
	class MTransform {
		public:
			MTransform();

			void translate(const Vector3 &translate);
			void rotate(const Vector3 &angle, float q);
			void scale(const Vector3 &s);

			Vector3 getTranslation() const {return transform.translation;}
			Vector3 getScale() const {return transform.scale;}
			Matrix& getTransformationMatrix();
			Eigen::Matrix4d getEigenTransformationMatrix();

		private:
			Matrix transformMat;
			Transform transform;
			bool updated;

			void updateMatrix();
		};
}
