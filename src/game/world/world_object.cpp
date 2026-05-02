#include "world_object.h"

WorldObject::~WorldObject() {
  delete m_pMesh;
  delete m_pShader;
}