#include "shader.h"

Shader::Shader(const std::string &shaderName)
{
  std::string shaderBasePath = "../shaders/";
  std::string vertex_filepath = shaderBasePath + shaderName + ".vert";
  std::string fragment_filepath = shaderBasePath + shaderName + ".frag";

  std::vector<unsigned int> modules = {
    compile(vertex_filepath, GL_VERTEX_SHADER),
    compile(fragment_filepath, GL_FRAGMENT_SHADER)
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

unsigned int Shader::compile(const std::string &filepath, unsigned int type) {
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

void Shader::use() const
{
  glUseProgram(m_RendererID);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
  int location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
  int location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform3fv(location, 1, &value[0]);
}

void Shader::setFloat(const std::string &name, float value) const
{
  int location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform1f(location, value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
  int location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform2fv(location, 1, &value[0]);
}