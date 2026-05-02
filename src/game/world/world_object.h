#pragma once
#include <include.h>
#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "renderer/render_context.h"
#include "math/aabb.h"

class WorldObject {
public:
  Mesh* m_pMesh;
  Shader* m_pShader;
  glm::vec3 m_Position, m_Rotation, m_Scale;
  int m_ObjectId;

  WorldObject(Mesh* mesh, Shader* shader, glm::vec3 pos = glm::vec3(0.0f))
    : m_pMesh(mesh), m_pShader(shader), m_Position(pos), m_Rotation(0.0f), m_Scale(1.0f) 
  {
    static int nextId = 0;
    m_ObjectId = nextId++;
  }

  ~WorldObject();

  auto draw(const RenderContext& context) -> void {
    m_pShader->use();

    // model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_Position);
    model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, m_Scale);

    m_pShader->setMat4("model", model);
    m_pShader->setMat4("view", context.view);
    m_pShader->setMat4("projection", context.projection);

    m_pMesh->draw();
  }

  auto setPosition(const glm::vec3& position) -> void { m_Position = position; }
  auto setRotation(const glm::vec3& rotation) -> void { m_Rotation = rotation; }
  auto setScale(const glm::vec3& scale) -> void { m_Scale = scale; }

  auto getAABB() -> AABB {
    // TODO: calculate the AABB based on the actual vertices of the mesh lol
    glm::vec3 halfScale = m_Scale * 0.5f;
    glm::vec3 outMin = m_Position - m_Scale;
    glm::vec3 outMax = m_Position + m_Scale;
    return { outMin, outMax };
  }
};
