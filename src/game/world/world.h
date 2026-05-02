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
  
  WorldObject* getObjectFromId(int id) {
    for (WorldObject* obj : m_Objects) {
      if (obj->m_ObjectId == id) {
        return obj;
      }
    }
    return nullptr;
  }

  void draw(const RenderContext& context) {
    for (WorldObject* obj : m_Objects) {
      // std::cout << "Drawing object at position: (" << obj->m_Position.x << ", " << obj->m_Position.y << ", " << obj->m_Position.z << ")" << std::endl;
      // std::cout << "drawing object with id: " << obj->m_ObjectId << std::endl;
      obj->draw(context);
    }
  }

  void loadLevel(PlayerController* playerController, const std::vector<std::string>& levelData) {
    clearObjects();

    // P = player start, D = door/exit
    // L = light, # = wall, . = floor

    for (size_t row = 0; row < levelData.size(); ++row) {
      const std::string& levelRow = levelData[row];
      for (size_t col = 0; col < levelRow.size(); ++col) {
        char tile = levelRow[col];
        if (tile == '#') {
          Mesh* wallMesh = Mesh::createCube();
          Shader* wallShader = new Shader("wallShader");
          WorldObject* wallObject = new WorldObject(wallMesh, wallShader, glm::vec3(col, row, 0.0f));
          addObject(wallObject);
        } else if (tile == 'P') {
          // start position
          playerController->setPosition(glm::vec3(col, row, 0.0f));
        }
      }
    }

    float floorAreaSize = 0.f;

    for (const std::string& row : levelData) {
      floorAreaSize = std::max(floorAreaSize, static_cast<float>(row.size()));
    }

    std::cout << "floor area size: " << floorAreaSize << std::endl;

    Mesh* floorMesh = Mesh::createQuad(floorAreaSize, floorAreaSize);
    Shader* floorShader = new Shader("floorShader");
    WorldObject* floorObject = new WorldObject(floorMesh, floorShader, glm::vec3(floorAreaSize / 2.0f - 0.5f, levelData.size() / 2.0f - 0.5f, -0.5f));
    floorObject->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    addObject(floorObject);
  }
};