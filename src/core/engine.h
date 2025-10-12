#pragma once
#include <include.h>
#include "camera.h"
#include "window.h"
#include "../renderer/shader.h"
#include "../input/input_manager.h"

class Engine
{
public:
  Window *m_pWindow;
  Camera *m_pCamera;
  InputManager* m_pInputManager;

  Engine();
  ~Engine();

  void run();
};