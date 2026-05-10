#include "shader.h"
#include "paths.h"

unsigned int Shader::s_BoundID = 0;

Shader::Shader(const std::string &shaderName)
{
  std::string vertexFilePath = Paths::Shaders + shaderName + ".vert";
  std::string fragmentFilePath = Paths::Shaders + shaderName + ".frag";

  std::vector<unsigned int> modules = {
    compile(vertexFilePath, GL_VERTEX_SHADER),
    compile(fragmentFilePath, GL_FRAGMENT_SHADER)
  };

  m_RendererID = glCreateProgram();
  for (unsigned int shaderModule : modules) {
    glAttachShader(m_RendererID, shaderModule);
  }
  glLinkProgram(m_RendererID);

  int success;
  glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
  if (!success) {
    char errorLog[512];
    glGetProgramInfoLog(m_RendererID, 512, NULL, errorLog);
    std::cout << "Shader linking failed:\n" << errorLog << std::endl;
  }

  for (unsigned int shaderModule : modules) {
    glDeleteShader(shaderModule);
  }
}

Shader::~Shader() {
  glDeleteProgram(m_RendererID);
}

auto Shader::compile(const std::string &filepath, unsigned int type) -> unsigned int {
  std::ifstream file(filepath);

  if (!file.is_open()) {
    std::cerr << "Failed to open shader file: " << filepath << std::endl;
    return 0;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string shaderSource = buffer.str();
  const char* shaderSrc = shaderSource.c_str();

  unsigned int shaderModule = glCreateShader(type);
  glShaderSource(shaderModule, 1, &shaderSrc, nullptr);
  glCompileShader(shaderModule);

  int success;
  glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
  if (!success) {
    char errorLog[512];
    glGetShaderInfoLog(shaderModule, 512, NULL, errorLog);
    std::cout << "Shader module compilation failed (" << filepath << "):\n" << errorLog << std::endl;
  }

  return shaderModule;
}

auto Shader::use() const -> void
{
  if (s_BoundID != m_RendererID) {
    glUseProgram(m_RendererID);
    s_BoundID = m_RendererID;
  }
}

auto Shader::setMat4(const std::string &name, const glm::mat4 &mat) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

auto Shader::setVec3(const std::string &name, const glm::vec3 &value) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform3fv(location, 1, &value[0]);
}

auto Shader::setFloat(const std::string &name, float value) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform1f(location, value);
}

auto Shader::setInt(const std::string &name, int value) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform1i(location, value);
}

auto Shader::setVec2(const std::string &name, const glm::vec2 &value) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform2fv(location, 1, &value[0]);
}

auto Shader::getUniformLocation(const std::string& name) const -> int {
  auto it = m_UniformCache.find(name);
  if (it != m_UniformCache.end()) return it->second;
  int loc = glGetUniformLocation(m_RendererID, name.c_str());
  m_UniformCache[name] = loc;
  return loc;
}