#pragma once
#include "engine.h"

namespace Debug
{
  extern bool bInWireframeMode;
  auto setupDebugConsole() -> void;
  auto registerDebugBindings(InputManager *pInput) -> void;
}
