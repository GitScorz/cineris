#pragma once
#include <vector>
#include "world_object.h"
#include "renderer/render_context.h"
#include "math/aabb.h"

class PlayerController;

class World {
public:
  std::vector<WorldObject*> m_Objects;
  World();
  ~World();

  auto addObject(WorldObject* object) -> void { m_Objects.push_back(object); }
  auto clearObjects() -> void {
    for (WorldObject* obj : m_Objects) {
      delete obj;
    }
    m_Objects.clear();
  }
  
  auto getObjectFromId(int id) -> WorldObject* {
    for (WorldObject* obj : m_Objects) {
      if (obj->m_ObjectId == id) {
        return obj;
      }
    }
    return nullptr;
  }

  auto draw(const RenderContext& context) -> void {
    for (WorldObject* obj : m_Objects) {
      // std::cout << "Drawing object at position: (" << obj->m_Position.x << ", " << obj->m_Position.y << ", " << obj->m_Position.z << ")" << std::endl;
      // std::cout << "drawing object with id: " << obj->m_ObjectId << std::endl;
      obj->draw(context);
    }
  }

  auto loadLevel(PlayerController* playerController, const std::vector<std::string>& levelData) -> void;

  auto collides(const AABB& box) -> bool {
    for (WorldObject* obj : m_Objects) {
      AABB objBox = obj->getAABB();
      if (box.min.x <= objBox.max.x && box.max.x >= objBox.min.x &&
          box.min.y <= objBox.max.y && box.max.y >= objBox.min.y &&
          box.min.z <= objBox.max.z && box.max.z >= objBox.min.z) {
        return true;
      }
    }
    return false;
  }
};
