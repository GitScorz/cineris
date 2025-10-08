#include "include.h"
#include "renderer/shader/shader.h"

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

  unsigned int shader = Shader::init();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader);
    glfwSwapBuffers(window);
  }

  glDeleteShader(shader);
  glfwTerminate();

  return 0;
}