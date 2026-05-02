#pragma once
#include <vector>
#include "world_object.h"
#include "renderer/render_context.h"
#include "../controllers/player_controller.h"

class World {
public:
  std::vector<WorldObject*> m_Objects;
  World();
  ~World();

  void addObject(WorldObject* object) { m_Objects.push_back(object); }
  void clearObjects() {
    for (WorldObject* obj : m_Objects) {
      delete obj;
    }
    m_Objects.clear();
  }

  void draw(const RenderContext& context) {
    for (WorldObject* obj : m_Objects) {
      // std::cout << "Drawing object at position: (" << obj->m_Position.x << ", " << obj->m_Position.y << ", " << obj->m_Position.z << ")" << std::endl;
      obj->draw(context);
    }
  }

  void loadLevel(PlayerController* playerController, const std::vector<std::string>& levelData) {
    clearObjects();

    for (size_t z = 0; z < levelData.size(); ++z) {
      const std::string& row = levelData[z];
      for (size_t x = 0; x < row.size(); ++x) {
        char tile = row[x];
        if (tile == '#') {
          Mesh* wallMesh = Mesh::createCube();
          Shader* wallShader = new Shader("wallShader");
          WorldObject* wallObject = new WorldObject(wallMesh, wallShader, glm::vec3(x, 0.0f, z));
          addObject(wallObject);
        } else if (tile == 'P') {
          playerController->setPosition(glm::vec3(x, 0.0f, z));
        }
      }
    }
  }
};