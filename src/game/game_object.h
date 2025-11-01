#pragma once
#include <include.h>
#include "../renderer/mesh.h"
#include "../renderer/shader.h"

class GameObject {
public:
  Mesh* m_pMesh;
  Shader* m_pShader;

  GameObject(Mesh* mesh, Shader* shader, glm::vec3 pos = glm::vec3(0.0f))
    : m_pMesh(mesh), m_pShader(shader), m_Position(pos), m_Rotation(0.0f), m_Scale(1.0f) {}

  ~GameObject();

  void draw() {
    m_pShader->use();

    // model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_Position);
    model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, m_Scale);

    m_pShader->setMat4("model", model);

    m_pMesh->draw();
  }

  void setPosition(const glm::vec3& position) { m_Position = position; }
  void setRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
  void setScale(const glm::vec3& scale) { m_Scale = scale; }

private:
  glm::vec3 m_Position, m_Rotation, m_Scale;
};