#pragma once
#include "engine.h"

namespace Debug
{
  extern bool bInWireframeMode;
  void setupDebugConsole();
  void registerDebugBindings(InputManager *pInput);
}