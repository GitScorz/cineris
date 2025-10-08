#include "config.h"

int main() {
  GLFWwindow* window;

  if (!glfwInit()) {
    std::cout << "GLFW couldn't start." << std::endl;
    return -1;
  }

  window = glfwCreateWindow(640, 480, "The Ashmoor Case", NULL, NULL);
  if (!window) {
    std::cout << "Failed to create GLFW window." << std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD." << std::endl;
    glfwTerminate();
    return -1;
  }

  glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}