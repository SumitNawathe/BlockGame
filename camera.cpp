#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Camera::updateCameraVectors() {
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	front(glm::vec3(0.0f, 0.0f, -1.0f)),
	movementSpeed(SPEED),
	mouseSensitivity(SENSITIVITY),
	zoom(ZOOM)
{
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ,
		float upX, float upY, float upZ,
		float yaw, float pitch) :
	front(glm::vec3(0.0f, 0.0f, -1.0f)),
	movementSpeed(SPEED),
	mouseSensitivity(SENSITIVITY),
	zoom(ZOOM)
{
	this->position = glm::vec3(posX, posY, posZ);
	this->worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

float Camera::getZoom() const {
	return zoom;
}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float dt) {
	float velocity = movementSpeed * dt;
	switch (direction) {
		case CameraMovement::FORWARD:	position += front * velocity; break;
		case CameraMovement::BACKWARD:	position -= front * velocity; break;
		case CameraMovement::LEFT:		position -= right * velocity; break;
		case CameraMovement::RIGHT:		position += right * velocity; break;
		case CameraMovement::UP:		position += worldUp * velocity; break;
		case CameraMovement::DOWN:		position -= worldUp * velocity; break;
	}
}

void Camera::processMouseMovement(float dx, float dy, GLboolean constrainPitch) {
	yaw += dx * mouseSensitivity;
	pitch += dy * mouseSensitivity;
	if (constrainPitch) {
		if (pitch > 89.0f) pitch = 89.0f;
		else if (pitch < -89.0f) pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::processMouseScroll(float dy) {
	zoom -= dy;
	if (zoom < 1.0f) zoom = 1.0f;
	else if (zoom > 45.0f) zoom = 45.0f;
}

