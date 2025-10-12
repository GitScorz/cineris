#pragma once
#include <vector>
#include "game_object.h"

class World {
public:
  std::vector<GameObject*> m_Objects;

  World();
  ~World();

  void addObject(GameObject* object) { m_Objects.push_back(object); }
  void clearObjects() {
    for (GameObject* obj : m_Objects) {
      delete obj;
    }
    m_Objects.clear();
  }

  void draw() {
    for (GameObject* obj : m_Objects) {
      obj->draw();
    }
  }
};