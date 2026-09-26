#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace cineris {

class Model;
class Shader;

class AssetManager {
public:
	std::shared_ptr<Model> loadModel(const std::string& path);
	std::shared_ptr<Shader> loadShader(const std::string& vertex,
									 const std::string& fragment);
	void clear();
	size_t modelCount() const { return m_models.size(); }

private:
	std::unordered_map<std::string, std::shared_ptr<Model>> m_models;
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};
}
