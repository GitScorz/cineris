#pragma once
#include <string>
#include <glm/glm.hpp>
#include <stdexcept>

namespace cineris {

class Shader {
public:
	unsigned int id;

	Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);
	~Shader();
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	void use() const;

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void setVec2(const std::string& name, float x, float y) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;

	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;

	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
private:
	enum class ShaderType {
		PROGRAM,
		VERTEX,
		FRAGMENT,
		GEOMETRY
	};

	std::string shaderTypeToString(ShaderType s) {
		switch (s) {
		case ShaderType::PROGRAM: return "PROGRAM";
		case ShaderType::VERTEX: return "VERTEX";
		case ShaderType::FRAGMENT: return "FRAGMENT";
		case ShaderType::GEOMETRY: return "GEOMETRY";
		default: throw std::invalid_argument("Invalid shader type");
		}
	}

	void checkCompileErrors(unsigned int shader, ShaderType type);
};

}
