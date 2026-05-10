#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "renderer/shader.h"
#include "renderer/mesh.h"
#include "renderer/texture.h"

class ResourceManager {
public:
  static ResourceManager& get() {
    static ResourceManager instance;
    return instance;
  }

  ~ResourceManager() = default;

  Shader* getShader(const std::string& name) {
    auto it = m_Shaders.find(name);
    if (it != m_Shaders.end()) return it->second.get();
    m_Shaders[name] = std::make_unique<Shader>(name);
    return m_Shaders[name].get();
  }

  Texture* getTexture(const std::string& name) {
    auto it = m_Textures.find(name);
    if (it != m_Textures.end()) return it->second.get();
    m_Textures[name] = std::make_unique<Texture>(name);
    return m_Textures[name].get();
  }
  
  Mesh* getCubeMesh() {
    if (!m_CubeMesh) m_CubeMesh = std::unique_ptr<Mesh>(Mesh::createCube());
    return m_CubeMesh.get();
  }

private:
  ResourceManager() = default;
  std::unordered_map<std::string, std::unique_ptr<Shader>> m_Shaders;
  std::unordered_map<std::string, std::unique_ptr<Texture>> m_Textures;
  std::unique_ptr<Mesh> m_CubeMesh;
};
