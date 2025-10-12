#pragma once
#include <include.h>
#include "camera.h"
#include "window.h"
#include "../renderer/shader.h"

class Engine
{
public:
  Window *m_pWindow;
  Camera *m_pCamera;

  Engine();
  ~Engine();

  void run();
};