#include "world.h"

World::World() {}

World::~World() {
  for (WorldObject* obj : m_Objects) {
    delete obj;
  }
  m_Objects.clear();
}