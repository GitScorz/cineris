#include <cineris/input/Input.h>

namespace cineris {

Input::Input(GLFWwindow* window) 
	: m_Window(window) 
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::update() {
	for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
		m_previous[key] = m_current[key];
		m_current[key] = glfwGetKey(m_Window, key) == GLFW_PRESS;
	}
}

bool Input::isPressed(int key) const {
	return m_current[key];
}

bool Input::wasPressed(int key) const {
	return m_current[key] && !m_previous[key];
}

bool Input::wasReleased(int key) const {
	return !m_current[key] && m_previous[key];
}
}
