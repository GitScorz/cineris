#pragma once
#include "include.h"
#include <string>

class Texture {
public:
  Texture(const std::string& filepath);
  ~Texture();
  
  auto bind(unsigned int slot = 0) const -> void;
  auto unbind() const -> void;

private:
  unsigned int m_RendererID = 0;
};