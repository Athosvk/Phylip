#include "camera.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace phyl{
	Camera::Camera(glm::vec3 pos, glm::vec3 up, float fov, float yaw, float pitch, float roll) 
		: position(pos), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(up), fov(fov), yaw(yaw), roll(roll), pitch(pitch) {
		updateVectors();
	}
	Camera::~Camera() {}
	float Camera::getFOV() const {
		return fov;
	}

	glm::mat4 Camera::getViewMatrix() const {
		return glm::lookAt(position, position + front, up);
	}

	glm::vec3& Camera::getPosition(){
		return position;
	}

	void Camera::updateVectors() {
		// The camera rotated and moved, new up and front vectors
		glm::vec3 newFront;
		newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		newFront.y = sin(glm::radians(pitch));
		newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(newFront);

		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

	void Camera::setPosition(glm::vec3 pos) {
		position = pos;
		//InputHandler::Instance()->scrollState(0.0);
		updateVectors();
	}
	void Camera::update(const float dt) {
		//auto inputHandler = InputHandler::Instance();
		//float scroll = inputHandler->getScrollState();
		//if(scroll != 0){
			//position += glm::vec3(.0f, .0f, (scroll > 0) ? -5.0f : 5.0f) * dt;
			//InputHandler::Instance()->scrollState(0.0);
			//updateVectors();
		//}

		//if(inputHandler->isKeyDown(KEYBOARD_W))
			//position += speed * up * dt;
		//if (inputHandler->isKeyDown(KEYBOARD_S))
			//position -= speed * up * dt;
		//if (inputHandler->isKeyDown(KEYBOARD_A))
			//position -= glm::normalize(glm::cross(front, up)) * speed * dt;
		//if (inputHandler->isKeyDown(KEYBOARD_D))
			//position += glm::normalize(glm::cross(front, up)) * speed * dt;
	}
};
