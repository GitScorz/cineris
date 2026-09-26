#pragma once
#include <GLFW/glfw3.h>
#include <vector>

namespace cineris {

class Input {
public:
	explicit Input(GLFWwindow* window);

	void update();

	bool isPressed(int key) const;
	bool wasPressed(int key) const;
	bool wasReleased(int key) const;

private:
	GLFWwindow* m_Window;

	std::vector<int> m_previous = std::vector<int>(GLFW_KEY_LAST + 1, 0);
	std::vector<int> m_current = std::vector<int>(GLFW_KEY_LAST + 1, 0);
};


}
