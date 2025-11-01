#pragma once
#include <include.h>

class Camera
{
public:
  float m_fYaw, m_fPitch;

  Camera(glm::vec3 defaultPos);
  ~Camera();

  glm::mat4 getViewMatrix();
  void updateCameraPosition(glm::vec3 vTarget);
  void rotate(float fDeltaYaw, float fDeltaPitch);

  glm::vec3 getFront() { return m_vFront; }
private:
  glm::vec3 m_vPosition, m_vFront, m_vUp, m_vTarget, m_vTargetOffset;
};
