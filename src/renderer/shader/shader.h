#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>

namespace Shader {
  unsigned int make_module(const std::string& filepath, unsigned int type);
  unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);

  unsigned int init();
}