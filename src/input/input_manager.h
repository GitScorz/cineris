#pragma once
#include <include.h>
#include <unordered_map>
#include <functional>

class InputManager {
public:
  static void processInput(GLFWwindow *window) {
    for (auto &[key, func] : s_mKeyBindings) {
      if (glfwGetKey(window, key) == GLFW_PRESS) {
        if (!s_mKeyStates[key]) {
          func();
          s_mKeyStates[key] = true;
        }
      } else {
        s_mKeyStates[key] = false;
      }
    }
  }

  static void registerKeyBinding(int key, std::function<void()> func) {
    s_mKeyBindings[key] = func;
    s_mKeyStates[key] = false; // Initialize key state
  }

  // static bool IsControlPressed(GLFWwindow *window, int key);
private:
  static std::unordered_map<int, bool> s_mKeyStates;
  static std::unordered_map<int, std::function<void()>> s_mKeyBindings;
};