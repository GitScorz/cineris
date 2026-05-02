#pragma once
#include <include.h>
#include <unordered_map>
#include <functional>

class InputManager {
public:
  bool m_bFirstMouse = true;
  double m_dLastMouseX = 0.0, m_dLastMouseY = 0.0;
  float m_fMouseSensitivity = 0.1f;

  InputManager(GLFWwindow* window);
  ~InputManager();

  auto processInput() -> void {
    for (auto &[key, func] : s_mKeyBindings) {
      if (glfwGetKey(m_pWindow, key) == GLFW_PRESS) {
        if (!s_mKeyStates[key]) {
          func();
          s_mKeyStates[key] = true;
        }
      } else {
        s_mKeyStates[key] = false;
      }
    }
  }

  static auto registerKeyBinding(int key, std::function<void()> func) -> void {
    s_mKeyBindings[key] = func;
    s_mKeyStates[key] = false; // Initialize key state
  }

  auto IsControlPressed(int key) -> bool {
    return glfwGetKey(m_pWindow, key) == GLFW_PRESS;
  }

  auto getMouseDeltaX() -> float {
    double xpos, ypos;
    glfwGetCursorPos(m_pWindow, &xpos, &ypos);
    float deltaX = xpos - m_dLastMouseX;
    m_dLastMouseX = xpos;
    return deltaX;
  }

  auto getMouseDeltaY() -> float {
    double xpos, ypos;
    glfwGetCursorPos(m_pWindow, &xpos, &ypos);
    float deltaY = ypos - m_dLastMouseY;
    m_dLastMouseY = ypos;
    return deltaY;
  }
private:
  GLFWwindow* m_pWindow;
  static std::unordered_map<int, bool> s_mKeyStates;
  static std::unordered_map<int, std::function<void()>> s_mKeyBindings;
};
