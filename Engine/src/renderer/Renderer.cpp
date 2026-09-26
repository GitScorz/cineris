#include <cineris/renderer/Renderer.h>
#include <cineris/renderer/Model.h>
#include <glad/glad.h>

namespace cineris {
Renderer::Renderer() { glEnable(GL_DEPTH_TEST); }
void Renderer::beginFrame() {
    glClearColor(0.025f, 0.025f, 0.035f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::draw(Model& model, Shader& shader, const Camera& camera,
                    const glm::mat4& transform) {
    shader.use();
    shader.setMat4("model", transform);
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", camera.getProjectionMatrix());
    model.draw(shader);
}
void Renderer::endFrame() {}
}
