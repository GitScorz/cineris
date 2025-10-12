#pragma once
#include <include.h>
#include "camera.h"
#include "window.h"
#include "renderer/shader.h"
#include "input/input_manager.h"
#include "game/world.h"
#include "player/player.h"

class Engine
{
public:
  Window *m_pWindow;
  Camera *m_pCamera;
  InputManager* m_pInputManager;
  World* m_pWorld;
  Player* m_pPlayer;

  double m_dLastFrame = 0.0, m_dDeltaTime = 0.0;

  Engine();
  ~Engine();

  void run();
  double calculateDeltaTime() {
    double dCurrentFrame = glfwGetTime();
    double dDeltaTime = dCurrentFrame - m_dLastFrame;
    m_dLastFrame = dCurrentFrame;

    if (dDeltaTime > 0.1) // max 100 ms per frame
      dDeltaTime = 0.1;

    return dDeltaTime;
  }
};