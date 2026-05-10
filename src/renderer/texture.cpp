#include "texture.h"
#include <iostream>
#include "paths.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& filepath) {
  std::string fullPath = Paths::Textures + filepath;

  glGenTextures(1, &m_RendererID);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);

  int width, height, nrChannels;
  unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);

  if (!data) {
    std::cerr << "Failed to load texture: " << filepath << std::endl;

    // clean up
    glDeleteTextures(1, &m_RendererID);
    m_RendererID = 0;

    return;
  }

  GLenum format = GL_RGB;
  if (nrChannels == 1) format = GL_RED;
  else if (nrChannels == 3) format = GL_RGB;
  else if (nrChannels == 4) format = GL_RGBA;

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
};

Texture::~Texture() {
  if (m_RendererID != 0) {
    glDeleteTextures(1, &m_RendererID);
  }
}

auto Texture::bind(unsigned int slot) const -> void {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);
};

auto Texture::unbind() const -> void {
  glBindTexture(GL_TEXTURE_2D, 0);
}