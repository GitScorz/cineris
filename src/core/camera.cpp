#include "camera.h"
#include <iostream>

Camera::Camera(glm::vec3 vTargetPos) 
  : m_vFront(glm::vec3(0.0f, 0.0f, -1.0f)), 
    m_vUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_fYaw(-90.0f), m_fPitch(0.0f), 
    m_vTargetOffset(glm::vec3(0.0f, 2.0f, 5.0f))
{
  updateCameraPosition(vTargetPos);
}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(m_vPosition, m_vPosition + m_vFront, m_vUp);
}

void Camera::updateCameraPosition(glm::vec3 vTarget) {
  m_vTarget = vTarget;
  m_vPosition = m_vTarget + m_vTargetOffset;
  m_vFront = glm::normalize(m_vTarget - m_vPosition);
}

void Camera::rotate(float fDeltaYaw, float fDeltaPitch)
{
  m_fYaw += fDeltaYaw;
  m_fPitch = glm::clamp(m_fPitch + fDeltaPitch, -89.0f, 89.0f);

  float fRadYaw = glm::radians(m_fYaw);
  float fRadPitch = glm::radians(m_fPitch);

  float fRadius = glm::length(m_vTargetOffset);
  m_vTargetOffset.x = fRadius * cos(fRadPitch) * cos(fRadYaw);
  m_vTargetOffset.y = fRadius * sin(fRadPitch);
  m_vTargetOffset.z = fRadius * cos(fRadPitch) * sin(fRadYaw);

  m_vPosition = m_vTarget + m_vTargetOffset;

  m_vFront = glm::normalize(m_vTarget - m_vPosition);
}
