#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

// camera movement directions
enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// default camera values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 6.0f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	float getZoom() const { return zoom; }
	glm::vec3 getPosition() const { return position; }
	glm::vec3 getFront() const { return front; }

	glm::mat4 getViewMatrix() const;

	void processKeyboard(CameraMovement direction, float dt);
	void processMouseMovement(float dx, float dy, GLboolean constrainPitch = true);
	void processMouseScroll(float dy);

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	void updateCameraVectors();
};

