#include "window.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
  glViewport(0, 0, width, height);
}

  Window::Window(int w, int h, const std::string &t)
      : m_Width(w), m_Height(h), m_Title(t)
  {
    if (!glfwInit())
    {
      std::cout << "GLFW couldn't start." << std::endl;
      exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  m_Handle = glfwCreateWindow(w, h, "The Ashmoor Case", NULL, NULL);
  if (!m_Handle)
  {
    std::cout << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(m_Handle);
  glfwSetFramebufferSizeCallback(m_Handle, framebuffer_size_callback);
  glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window() {
  glfwDestroyWindow(m_Handle);
  glfwTerminate();
}

void Window::update() {
  glfwSwapBuffers(m_Handle);
  glfwPollEvents();
}

bool Window::shouldClose() const {
  return glfwWindowShouldClose(m_Handle);
}