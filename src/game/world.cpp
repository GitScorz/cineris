#include "world.h"

World::World() {}

World::~World() {
  for (GameObject* obj : m_Objects) {
    delete obj;
  }
  m_Objects.clear();
}