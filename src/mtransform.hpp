#pragma once

#include "raylib.h"
#include "raymath.h"

namespace phyl {
	class MTransform {
		public:
			MTransform();
			Matrix& getTransformationMatrix();

			void translate(const Vector3 &translate);
			void rotate(const Vector3 &angle, float q);
			void scale(const Vector3 &s);
		private:
			Matrix transformMat;
			Transform transform;
			bool updated;

			void updateMatrix();
		};
}
