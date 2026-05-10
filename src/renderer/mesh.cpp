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

  GLsizei stride = 8 * sizeof(float);

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
  glEnableVertexAttribArray(0);

  // normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // uv
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

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
    // positions          // normals           // uv

    // back face
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
    // front face
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
    // left face
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
    // right face
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
    // bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
    // top face
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,     0.0f, 1.0f,
  };

  std::vector<unsigned int> inds = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20
  };

  return new Mesh(verts, inds);
}

auto Mesh::createQuad(float fWidth, float fHeight) -> Mesh* {
  float w = fWidth / 2.0f;
  float h = fHeight / 2.0f;

  std::vector<float> verts = {
    // positions      // normals (up)     // uv
    -w, 0.0f, -h,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    w, 0.0f, -h,     0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
    w, 0.0f,  h,     0.0f, 1.0f, 0.0f,    1.0f, 1.0f,
    -w, 0.0f,  h,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f
  };

  std::vector<unsigned int> inds = {
    0, 1, 2,
    2, 3, 0
  };

  return new Mesh(verts, inds);
}

auto Mesh::createGrid(float fWidth, float fDepth, int iRows, int iCols) -> Mesh* {
  std::vector<float> verts = {};
  std::vector<unsigned int> inds = {};

  for (int row = 0; row < iRows; row++) {
    for (int col = 0; col < iCols; col++) {

      float uPos = static_cast<float>(col / static_cast<float>(iCols - 1));
      float vPos = static_cast<float>(row / static_cast<float>(iRows - 1));

      float x = uPos * fWidth - fWidth / 2;
      float z = vPos * fDepth - fDepth / 2;
      // float y = 0.f;
      float y = sin(x * 0.3f) * cos(z * 0.3f) * 2.0f;

      // pos
      verts.push_back(x);
      verts.push_back(y);
      verts.push_back(z);

      // normals
      verts.push_back(0.f);
      verts.push_back(1.f);
      verts.push_back(0.f);

      // uv
      verts.push_back(uPos);
      verts.push_back(vPos);
    }
  }

  for (int row = 0; row < iRows - 1; row++) {
    for (int col = 0; col < iCols - 1; col++) {
      unsigned int topLeft = row * iCols + col;
      unsigned int topRight = topLeft + 1;
      unsigned int bottomLeft = (row + 1) * iCols + col;
      unsigned int bottomRight = bottomLeft + 1;

      inds.push_back(topLeft);
      inds.push_back(bottomLeft);
      inds.push_back(topRight);

      inds.push_back(topRight);
      inds.push_back(bottomLeft);
      inds.push_back(bottomRight);
    }
  }

  return new Mesh(verts, inds);
}