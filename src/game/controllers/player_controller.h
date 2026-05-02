#pragma once
#include <include.h>
#include "core/camera.h"
#include "input/input_manager.h"

class PlayerController
{
public:
  float m_fHealth = 100.f, m_fSpeed = 0.f;

  PlayerController(Camera *pCamera, InputManager *pInputManager);
  ~PlayerController();

  void update(float fDeltaTime);

  void setPosition(glm::vec3 vPos) { m_vPosition = vPos; }
  glm::vec3 getPosition() { return m_vPosition; }

  glm::vec3 getVelocity() { return m_vVelocity; }
  void setVelocity(glm::vec3 vVel) { m_vVelocity = vVel; }

private:
  Camera* m_pCamera = nullptr;
  InputManager* m_pInputManager = nullptr;

  glm::vec3 m_vPosition{};
  glm::vec3 m_vVelocity{};
};
