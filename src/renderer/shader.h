#pragma once
#include <include.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

class Shader {
  public:
    Shader(const std::string& shaderName);
    ~Shader();

    auto use() const -> void;
    auto setMat4(const std::string &name, const glm::mat4 &mat) const -> void;
    auto setVec3(const std::string &name, const glm::vec3 &value) const -> void;
    auto setVec2(const std::string &name, const glm::vec2 &value) const -> void;
    auto setFloat(const std::string &name, float value) const -> void;

    auto getID() const -> unsigned int { 
      return m_RendererID; 
    }

  private:
    unsigned int m_RendererID = 0;
    std::string m_VertexSource;
    std::string m_FragmentSource;

    auto compile(const std::string &filepath, unsigned int type) -> unsigned int;
};
