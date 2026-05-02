#include "input_manager.h"

std::unordered_map<int, bool> InputManager::s_mKeyStates;
std::unordered_map<int, std::function<void()>> InputManager::s_mKeyBindings;

InputManager::InputManager(GLFWwindow* window) 
  : m_pWindow(window)
{
  if (m_bFirstMouse) {
    glfwGetCursorPos(m_pWindow, &m_dLastMouseX, &m_dLastMouseY);
    m_bFirstMouse = false;
  }
}

InputManager::~InputManager() {}
