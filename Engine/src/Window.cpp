#include <cineris/Window.h>
#include <iostream>

namespace cineris {

Window::Window(const unsigned int w, const unsigned int h, const std::string& title)
	: m_Width(w), m_Height(h), m_Title(title) 
{
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Handle = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);

	if (m_Handle == nullptr) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(m_Handle);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwDestroyWindow(m_Handle);
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetWindowUserPointer(m_Handle, this);

	glfwSetFramebufferSizeCallback(m_Handle, framebufferSizeCallback);

	m_Input = std::make_unique<Input>(m_Handle);
}

Window::~Window() {
	glfwDestroyWindow(m_Handle);
	glfwTerminate();
}

void Window::update() {
	glfwSwapBuffers(m_Handle);
}

void Window::pollEvents() {
	glfwPollEvents();
	m_Input->update();
}

bool Window::open() const {
	return !glfwWindowShouldClose(m_Handle);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (win) {
		win->m_Width = width;
		win->m_Height = height;
		glViewport(0, 0, width, height);
	}
}

GLFWwindow* Window::handle() const {
	return m_Handle;
}

}
