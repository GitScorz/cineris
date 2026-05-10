#pragma once
#include <include.h>
#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "renderer/render_context.h"
#include "math/aabb.h"
#include "renderer/texture.h"

class WorldObject {
public:
  Mesh* m_pMesh;
  Shader* m_pShader;
  Texture* m_pTexture;

  WorldObject(Mesh* mesh, Shader* shader, Texture* texture, glm::vec3 pos = glm::vec3(0.0f))
    : m_pMesh(mesh), m_pShader(shader), m_pTexture(texture), m_Position(pos), m_Rotation(0.0f), m_Scale(1.0f), m_ObjectColor(1.0f) 
  {
    static int nextId = 0;
    m_ObjectId = nextId++;
  }

  ~WorldObject() = default;

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
    m_pShader->setVec3("viewPos", context.viewPos);
    m_pShader->setVec3("lightColor", context.lightColor);
    m_pShader->setVec3("objectColor", m_ObjectColor);

    int lightCount = static_cast<int>(context.lightPositions.size());
    if (lightCount > RenderContext::MAX_LIGHTS) {
      lightCount = RenderContext::MAX_LIGHTS;
    }
    m_pShader->setInt("lightCount", lightCount);

    for (int i = 0; i < lightCount; ++i) {
      m_pShader->setVec3("lightPositions[" + std::to_string(i) + "]", context.lightPositions[i]);
    }

    m_pTexture->bind(0);
    m_pMesh->draw();
  }

  auto setPosition(const glm::vec3& position) -> void { m_Position = position; }
  auto setRotation(const glm::vec3& rotation) -> void { m_Rotation = rotation; }
  auto setScale(const glm::vec3& scale) -> void { m_Scale = scale; }
  auto setObjectColor(const glm::vec3& color) -> void { m_ObjectColor = color; }
  auto setIsLightSource(bool isLight) -> void { m_bIsLightSource = isLight; }
  auto setIsCollidable(bool collidable) -> void { m_bIsCollidable = collidable; }

  auto isLightSource() const -> bool { return m_bIsLightSource; }

  auto getAABB() -> AABB {
    // TODO: calculate the AABB based on the actual vertices of the mesh lol
    glm::vec3 outMin = m_Position - m_Scale;
    glm::vec3 outMax = m_Position + m_Scale;
    return { outMin, outMax };
  }

  auto getId() const -> int { return m_ObjectId; }
  auto isCollidable() const -> bool { return m_bIsCollidable; }
  auto getPosition() const -> glm::vec3 { return m_Position; }
  auto getRotation() const -> glm::vec3 { return m_Rotation; }
  auto getScale() const -> glm::vec3 { return m_Scale; }
  auto getObjectColor() const -> glm::vec3 { return m_ObjectColor; }
private:
  glm::vec3 m_Position, m_Rotation, m_Scale;
  glm::vec3 m_ObjectColor;
  int m_ObjectId;

  bool m_bIsLightSource = false;
  bool m_bIsCollidable = true;
};
