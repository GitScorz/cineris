#include "shader.h"

unsigned int Shader::make_module(const std::string& filepath, unsigned int type) {
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

unsigned int Shader::make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath) {
  std::vector<unsigned int> modules = {
    make_module(vertex_filepath, GL_VERTEX_SHADER),
    make_module(fragment_filepath, GL_FRAGMENT_SHADER)
  };

  unsigned int shader = glCreateProgram();
  for (unsigned int shaderModule : modules) {
    glAttachShader(shader, shaderModule);
  }

  glLinkProgram(shader);

  int success;
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if (!success) {
    char errorLog[512];
    glGetProgramInfoLog(shader, 512, NULL, errorLog);
    std::cout << "Shader linking failed\n" << errorLog << std::endl;
  }

  for (unsigned int shaderModule : modules) {
    glDeleteShader(shaderModule);
  }

  return shader;
}

unsigned int Shader::init() {
  unsigned int shader = make_shader(
    "../shaders/vertex.glsl",
    "../shaders/fragment.glsl"
  );
  
  return shader;
}