#pragma once
#include <cineris/renderer/Shader.h>
#include <cineris/renderer/Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace cineris {

class Model {
public:
	Model(const std::string& filepath);
	~Model();
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	void draw(Shader& shader, const std::vector<Material>& overrides = {});
	const std::vector<Material>& getMaterials() const { return materials; }

private:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	std::string directory;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
}
