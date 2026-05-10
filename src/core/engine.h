#pragma once
#include <include.h>
#include "camera.h"
#include "window.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "input/input_manager.h"
#include "game/world/world.h"
#include "game/controllers/player_controller.h"

class Cineris
{
public:
  Window *m_pWindow = nullptr;
  Camera *m_pCamera = nullptr;
  InputManager* m_pInputManager = nullptr;
  World* m_pWorld = nullptr;
  PlayerController* m_pPlayer = nullptr;
  double m_dLastFrame = 0.0, m_dDeltaTime = 0.0;

  Cineris(const std::string& title);
  ~Cineris();

  auto run() -> void;
  auto calculateDeltaTime() -> double {
    double dCurrentFrame = glfwGetTime();
    double dDeltaTime = dCurrentFrame - m_dLastFrame;
    m_dLastFrame = dCurrentFrame;

    if (dDeltaTime > 0.1) // max 100 ms per frame
      dDeltaTime = 0.1;

    return dDeltaTime;
  }
};
