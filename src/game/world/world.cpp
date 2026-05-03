#include "world.h"
#include "../controllers/player_controller.h"

World::World() {}

World::~World() {
  for (WorldObject* obj : m_Objects) {
    delete obj;
  }
  m_Objects.clear();
}

auto World::loadLevel(PlayerController* playerController, const std::vector<std::string>& levelData) -> void {
  clearObjects();

  // P = player start, D = door/exit
  // L = light, # = wall, . = floor

  for (size_t row = 0; row < levelData.size(); ++row) {
    const std::string& levelRow = levelData[row];
    for (size_t col = 0; col < levelRow.size(); ++col) {
      char tile = levelRow[col];
      if (tile == '#') {
        Mesh* wallMesh = Mesh::createCube();
        Shader* wallShader = new Shader("lightning");
        WorldObject* wallObject = new WorldObject(wallMesh, wallShader, glm::vec3(col, row, 0.0f));
        wallObject->setObjectColor(glm::vec3(0.6f, 0.6f, 0.65f));
        addObject(wallObject);
      } else if (tile == 'P') {
        // start position
        playerController->setPosition(glm::vec3(col, row, 0.0f));
      } else if (tile == 'L') {
        // light source
        Mesh* lightMesh = Mesh::createCube();
        Shader* lightShader = new Shader("cube");
        WorldObject* lightObject = new WorldObject(lightMesh, lightShader, glm::vec3(col, row, 0.0f));
        lightObject->setScale(glm::vec3(0.2f));
        lightObject->setIsLightSource(true);
        addObject(lightObject);
      }
    }
  }

  float floorAreaSize = 0.f;

  for (const std::string& row : levelData) {
    floorAreaSize = std::max(floorAreaSize, static_cast<float>(row.size()));
  }

  std::cout << "floor area size: " << floorAreaSize << std::endl;

  Mesh* floorMesh = Mesh::createQuad(floorAreaSize, floorAreaSize);
  Shader* floorShader = new Shader("lightning");
  WorldObject* floorObject = new WorldObject(floorMesh, floorShader, glm::vec3(floorAreaSize / 2.0f - 0.5f, levelData.size() / 2.0f - 0.5f, -0.5f));
  floorObject->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
  floorObject->setObjectColor(glm::vec3(0.25f, 0.35f, 0.25f));
  addObject(floorObject);
}
