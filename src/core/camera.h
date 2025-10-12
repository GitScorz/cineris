#pragma once
#include <include.h>

class Camera
{
public:
  glm::vec3 m_Position, m_Front, m_Up;
  float m_fYaw, m_fPitch;
  // float m_fMovementSpeed, m_fMouseSensitivity, m_fZoom;

  Camera(glm::vec3 defaultPos);

  glm::mat4 getViewMatrix();
};
