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
  void draw();

  static Mesh* createCube();
};