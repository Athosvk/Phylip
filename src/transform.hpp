#pragma once

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

namespace phyl {
	class Transform {
		public:
			Transform();
			const glm::mat4& getTransformationMatrix();

			void translate(const glm::vec3 &translate);
			void rotate(const glm::vec3 &angle, float q);
			void scale(const glm::vec3 &s);
		private:
			glm::mat4 transformMat;
			glm::vec3 translation;
			glm::vec3 scaling;
			glm::quat rotation;
			bool updated;

			void updateMatrix();
		};
}
