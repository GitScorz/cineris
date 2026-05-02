#pragma once
#include <vector>
#include "world_object.h"

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

  void draw() {
    for (WorldObject* obj : m_Objects) {
      obj->draw();
    }
  }
};