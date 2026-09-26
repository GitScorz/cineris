#pragma once
#include <cineris/camera/Camera.h>

namespace cineris {
class Model;
class Shader;

// OpenGL implementation; the caller owns scene content and camera controls.
class Renderer {
public:
    Renderer();
    void beginFrame();
    void draw(Model& model, Shader& shader, const Camera& camera,
              const glm::mat4& transform = glm::mat4(1.0f));
    void endFrame();
};
}
