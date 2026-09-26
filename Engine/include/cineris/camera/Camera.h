#pragma once
#include <cineris/math/GLM.h>

namespace cineris {

class Camera {
public:
	Camera(float fov, float aspectRatio, float nearPlane, float farPlane, glm::vec3 position = glm::vec3(0.0f))
		: m_fov(fov), 
		m_aspectRatio(aspectRatio), 
		m_nearPlane(nearPlane), 
		m_farPlane(farPlane),
		m_position(position) {}

	void update();
	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch);
	void moveForward(float distance) { m_position += m_forward * distance; }
	void moveRight(float distance) { m_position += m_right * distance; }
	void moveUp(float distance) { m_position.y += distance; }

	void setPosition(const glm::vec3& position) { m_position = position; }
	glm::vec3 getPosition() const { return m_position; }
	glm::vec3 getRotation() const { return m_rotation; }

	void setRotation(const glm::vec3& rotation) { m_rotation = rotation; update(); }
	void setRotation(float pitch, float yaw) { m_rotation.x = pitch; m_rotation.y = yaw; update(); }

	void setFOV(float fov) { m_fov = fov; }
	void setSensivity(float sensivity) { m_sensivity = sensivity; }
	void setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
	void setMouseMovementEnabled(bool enabled) { m_mouseMovementEnabled = enabled; }

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;


private:
	float m_fov;
	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;
	float m_sensivity = 0.0f;

	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f, -90.0f, 0.0f);

	glm::vec3 m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);

	bool m_mouseMovementEnabled = true;
};

}
