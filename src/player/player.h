#pragma once
#include <include.h>
#include "core/camera.h"
#include "input/input_manager.h"

class Player
{
public:
  float m_fHealth, m_fSpeed;

  Player(Camera *pCamera, InputManager *pInputManager);
  ~Player();

  void update(float fDeltaTime);

  void setPosition(glm::vec3 vPos) { m_vPosition = vPos; }
  glm::vec3 getPosition() { return m_vPosition; }

  glm::vec3 getVelocity() { return m_vVelocity; }
  void setVelocity(glm::vec3 vVel) { m_vVelocity = vVel; }

private:
  Camera* m_pCamera;
  InputManager* m_pInputManager;

  glm::vec3 m_vPosition, m_vVelocity;
};
