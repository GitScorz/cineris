#pragma once
#include <include.h>

class Camera
{
public:
  float m_fYaw, m_fPitch;

  Camera(glm::vec3 defaultPos);
  ~Camera();

  auto getViewMatrix() const -> glm::mat4;
  auto updateCameraPosition(glm::vec3 vTarget) -> void;
  auto rotate(float fDeltaYaw, float fDeltaPitch) -> void;
  
  auto getPosition() -> glm::vec3 { return m_vPosition; }
  auto getFront() -> glm::vec3 { return m_vFront; }
private:
  glm::vec3 m_vPosition, m_vFront, m_vUp, m_vTarget, m_vTargetOffset;
};
