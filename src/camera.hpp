#pragma once

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace phyl {
	class Camera {
		public:
			Camera(glm::vec3 pos, glm::vec3 up, float fov, float yaw, float pitch, float roll);
			~Camera();

			float getFOV() const;
			glm::mat4 getViewMatrix() const;
			glm::vec3& getPosition();

			void setPosition(glm::vec3 pos);
			void reset();

			void update(const float dt);

		protected:
			void updateVectors();

			glm::vec3 position;
			glm::vec3 front;
			glm::vec3 right;
			glm::vec3 up;
			glm::vec3 worldUp;

			float fov;
			float yaw;
			float roll;
			float pitch;
			const float speed = 2.0f;
	};
};
