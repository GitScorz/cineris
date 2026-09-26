#include <cineris/assets/AssetManager.h>
#include <cineris/renderer/Model.h>
#include <cineris/renderer/Shader.h>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace cineris {

namespace {

std::string keyFor(const std::string& path) {
	auto result = std::filesystem::weakly_canonical(path).generic_string();

	// windows paths are case insensitive for this engine's supported setup
	std::transform(result.begin(), result.end(), result.begin(),
		[](unsigned char c) {
			return static_cast<char>(std::tolower(c)); 
		});

	return result;
}
}

std::shared_ptr<Model> AssetManager::loadModel(const std::string& path) {
	const auto key = keyFor(path);
	if (const auto found = m_models.find(key); found != m_models.end())
		return found->second;

	auto model = std::make_shared<Model>(path);
	m_models.emplace(key, model);
	return model;
}

std::shared_ptr<Shader> AssetManager::loadShader(const std::string& vertex,
	const std::string& fragment) {
	const auto key = keyFor(vertex) + '\n' + keyFor(fragment);
	if (const auto found = m_shaders.find(key); found != m_shaders.end())
		return found->second;

	auto shader = std::make_shared<Shader>(vertex, fragment, "");
	m_shaders.emplace(key, shader);
	return shader;
}

void AssetManager::clear() {
	m_models.clear();
	m_shaders.clear();
}
}
