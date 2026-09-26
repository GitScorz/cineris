#include <cineris/camera/Camera.h>
#include <glad/glad.h>
#include <iostream>

namespace cineris {

void Camera::update() {
	glm::vec3 front(0.0f);
	front.x = cos(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));
	front.y = sin(glm::radians(m_rotation.x));
	front.z = sin(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));

	m_forward = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_forward));
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
	if (!m_mouseMovementEnabled) return;
	
	xoffset *= m_sensivity;
    yoffset *= m_sensivity;

    m_rotation.y += xoffset; // yaw
    m_rotation.x += yoffset; // pitch

    if (constrainPitch) {
		m_rotation.x = glm::clamp(m_rotation.x, -89.0f, 89.0f);
    }

    update();
}

glm::mat4 Camera::getViewMatrix() const {
	glm::vec3 center = m_position + m_forward;
	return glm::lookAt(m_position, center, m_up);
}

glm::mat4 Camera::getProjectionMatrix() const {
	return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
}
}
