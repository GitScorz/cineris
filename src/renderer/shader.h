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
    Shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
    ~Shader();

    void use() const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setFloat(const std::string &name, float value) const;

    unsigned int getID() const { return m_RendererID; }

  private:
    unsigned int m_RendererID;
    std::string m_VertexSource;
    std::string m_FragmentSource;

    unsigned int compile(const std::string &filepath, unsigned int type);
};