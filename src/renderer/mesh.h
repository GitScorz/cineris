#pragma once
#include <include.h>
#include <vector>

class Mesh {
public:
  // Vertex Buffer Object, Vertex Array Object and Element Buffer Object
  unsigned int VBO, VAO, EBO;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;

  Mesh(std::vector<float> verts, std::vector<unsigned int> inds);
  ~Mesh();
  auto draw() -> void;

  static auto createCube() -> Mesh*;
  static auto createQuad(float fWidth = 1.0f, float fHeight = 1.0f) -> Mesh*;
};
