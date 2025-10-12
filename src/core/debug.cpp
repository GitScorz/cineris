#include "debug.h"

namespace Debug
{
  bool bInWireframeMode = false;

  void registerDebugBindings(InputManager *pInput) {
    pInput->registerKeyBinding(GLFW_KEY_F1, []() {
      bInWireframeMode = !bInWireframeMode;
      glPolygonMode(GL_FRONT_AND_BACK, bInWireframeMode ? GL_LINE : GL_FILL);
    });
    
    pInput->registerKeyBinding(GLFW_KEY_ESCAPE, [](){ 
      glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
    });
  }
}