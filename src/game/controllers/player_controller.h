#pragma once
#include <include.h>
#include "core/camera.h"
#include "input/input_manager.h"
#include "math/aabb.h"

class World;

class PlayerController
{
public:
  float m_fHealth = 100.f, m_fSpeed = 0.f;

  PlayerController(Camera *pCamera, InputManager *pInputManager);
  ~PlayerController();

  auto update(float fDeltaTime, World* pWorld) -> void;
  auto getAABBAt(glm::vec3 vPosition) -> AABB;

  auto setPosition(glm::vec3 vPos) -> void { m_vPosition = vPos; }
  auto getPosition() -> glm::vec3 { return m_vPosition; }

  auto getVelocity() -> glm::vec3 { return m_vVelocity; }
  auto setVelocity(glm::vec3 vVel) -> void { m_vVelocity = vVel; }

private:
  Camera* m_pCamera = nullptr;
  InputManager* m_pInputManager = nullptr;

  glm::vec3 m_vPosition{};
  glm::vec3 m_vVelocity{};
};
