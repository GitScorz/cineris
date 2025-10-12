#include "input_manager.h"

std::unordered_map<int, bool> InputManager::s_mKeyStates;
std::unordered_map<int, std::function<void()>> InputManager::s_mKeyBindings;