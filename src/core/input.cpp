#include "input.h"

bool Input::IsControlPressed(GLFWwindow *window, int key) {
  return glfwGetKey(window, key) == GLFW_PRESS;
}

// bool inWireframeMode = false;

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//   if (action == GLFW_PRESS) {
//     if (key == GLFW_KEY_ESCAPE)
//       glfwSetWindowShouldClose(window, true);
//   } else if (action == GLFW_RELEASE) {
//     if (key == GLFW_KEY_F7) {
//       inWireframeMode = !inWireframeMode;

//       if (inWireframeMode)
//         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//       else
//         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//     }
//   }
// }

// glfwSetKeyCallback(window, key_callback);
