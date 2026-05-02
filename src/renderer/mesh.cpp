#include "mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<float> verts, std::vector<unsigned int> inds)
    : vertices(verts), indices(inds)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

auto Mesh::draw() -> void
{
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

auto Mesh::createCube() -> Mesh* {
  std::vector<float> verts = {
    // positions
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f
  };

  std::vector<unsigned int> inds = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    0, 4, 7, 7, 3, 0,
    1, 5, 6, 6, 2, 1,
    3, 2, 6, 6, 7, 3,
    0, 1, 5, 5, 4, 0 
  };

  return new Mesh(verts, inds);
}

auto Mesh::createQuad(float fWidth, float fHeight) -> Mesh* {
  float w = fWidth / 2.0f;
  float h = fHeight / 2.0f;

  std::vector<float> verts = {
    -w, 0.0f, -h, 
    w, 0.0f, -h, 
    w, 0.0f, h, 
    -w, 0.0f, h
  };

  std::vector<unsigned int> inds = {
    0, 1, 2,
    2, 3, 0
  };

  return new Mesh(verts, inds);
}
