#include "world.h"
#include "game/controllers/player_controller.h"
#include "core/resource_manager.h"

World::World() {}

World::~World() {
  for (WorldObject* obj : m_Objects) {
    delete obj;
  }
  m_Objects.clear();
}

auto World::loadLevel(PlayerController* playerController, const std::vector<std::string>& levelData) -> void {
  clearObjects();

  for (size_t row = 0; row < levelData.size(); ++row) {
    const std::string& levelRow = levelData[row];
    for (size_t col = 0; col < levelRow.size(); ++col) {
      char tile = levelRow[col];
      if (tile == '#') {
        Mesh* wallMesh = ResourceManager::get().getCubeMesh();
        Shader* wallShader = ResourceManager::get().getShader("lightning");
        WorldObject* wallObject = new WorldObject(wallMesh, wallShader, nullptr, glm::vec3(col, row, 0.0f));
        wallObject->setObjectColor(glm::vec3(0.6f, 0.6f, 0.65f));
        addObject(wallObject);
      } else if (tile == 'P') {
        // start position
        playerController->setPosition(glm::vec3(col, row, 0.0f));
      } else if (tile == 'L') {
        // light source
        Mesh* lightMesh = Mesh::createCube();
        Shader* lightShader = ResourceManager::get().getShader("cube");
        WorldObject* lightObject = new WorldObject(lightMesh, lightShader, nullptr, glm::vec3(col, row, 0.0f));
        lightObject->setScale(glm::vec3(0.2f));
        lightObject->setIsLightSource(true);
        addObject(lightObject);
      }
    }
  }

  float floorAreaSize = 20.f;

  for (const std::string& row : levelData) {
    floorAreaSize = std::max(floorAreaSize, static_cast<float>(row.size()));
  }

  std::cout << "floor area size: " << floorAreaSize << std::endl;

  Mesh* floorMesh = Mesh::createQuad(floorAreaSize, floorAreaSize);
  Shader* floorShader = ResourceManager::get().getShader("lightning");
  Texture* floorTexture = ResourceManager::get().getTexture("Rocks024L_1K/Rocks024L_1K-PNG_Color.png");
  WorldObject* floorObject = new WorldObject(floorMesh, floorShader, floorTexture, glm::vec3(floorAreaSize / 2.0f - 0.5f, levelData.size() / 2.0f - 0.5f, -0.5f));
  floorObject->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
  floorObject->setObjectColor(glm::vec3(0.25f, 0.35f, 0.25f));
  addObject(floorObject);
}
