#pragma once
#include <include.h>
#include "core/camera.h"
#include "input/input_manager.h"

class Player
{
public:
  glm::vec3 m_vPosition, m_vVelocity;
  float m_fHealth, m_fSpeed;

  Player(Camera *pCamera, InputManager *pInputManager);
  ~Player();

  void update(float fDeltaTime);

private:
  Camera* m_pCamera;
  InputManager* m_pInputManager;
};
