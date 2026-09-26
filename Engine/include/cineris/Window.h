#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cineris/input/Input.h>
#include <string>
#include <memory>

namespace cineris {

class Window {
public:

	Window(const unsigned int w, const unsigned int h, const std::string& title);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	bool open() const;

	GLFWwindow* handle() const;
	void update();
	void pollEvents();

	int getWidth() const {
		return m_Width;
	};

	int getHeight() const {
		return m_Height;
	};

	Input* input() const {
		return m_Input.get();
	};

private:
	unsigned int m_Width, m_Height;

	GLFWwindow* m_Handle;
	std::string m_Title;
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	std::unique_ptr<Input> m_Input;
};
}
