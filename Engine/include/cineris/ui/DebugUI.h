#pragma once
#include <cineris/common/UI.h>
#include <cineris/Window.h>
#include <GLFW/glfw3.h>

namespace cineris {

namespace DebugUI {
	void Init(GLFWwindow* window);
	void Shutdown();

	void StartFrame();
	void EndFrame();
}
}
