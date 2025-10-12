#include "camera.h"
#include <iostream>

Camera::Camera(glm::vec3 defaultPos) 
  : m_Position(defaultPos), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_fYaw(-90.0f), m_fPitch(0.0f)
{}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}