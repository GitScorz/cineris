#include "core/engine.h"

// TODO: Create a resource manager to handle meshes, textures, shaders, etc. for better memory management.

int main() {
  Cineris engine = Cineris("The Ashmoor Case");
  engine.run();
  return 0;
}
