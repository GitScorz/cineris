#include <cineris/renderer/Model.h>
#include <iostream>
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <filesystem>
#include <algorithm>
#include <cmath>

namespace cineris {

unsigned int TextureFromFile(const char* path, const std::string& directory, bool srgb) {
	const auto filename = (std::filesystem::path(directory) / path).string();
	int width, height, components;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &components, 4);

	if (!data) {
		std::cerr << "Texture failed to load: " << filename << std::endl;
		return 0;
	}

	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8,
		width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

	return textureID;
}

Model::Model(const std::string& filepath) {
	Assimp::Importer importer;
	// Bake imported node transforms for the current static-mesh renderer.
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error("Cannot load model " + filepath + ": " + importer.GetErrorString());
	}

	directory = std::filesystem::path(filepath).parent_path().string();

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
		const auto* source = scene->mMaterials[i];
		Material material;
		aiString name;
		source->Get(AI_MATKEY_NAME, name);
		material.name = name.length ? name.C_Str() : "Material " + std::to_string(i);

		aiColor3D color(1.0f, 1.0f, 1.0f);
		source->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material.tint = glm::vec3(color.r, color.g, color.b);
		source->Get(AI_MATKEY_OPACITY, material.opacity);

		aiColor3D specular;
		if (source->Get(AI_MATKEY_COLOR_SPECULAR, specular) == AI_SUCCESS) {
			material.specular = std::clamp((specular.r + specular.g + specular.b) / 3.0f, 0.0f, 1.0f);
		}

		float shininess = 0.0f;
		if (source->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
			material.roughness = std::clamp(std::sqrt(2.0f / (std::max(shininess, 0.0f) + 2.0f)), 0.05f, 1.0f);
		}
		source->Get(AI_MATKEY_ROUGHNESS_FACTOR, material.roughness);
		materials.push_back(material);
	}

	if (materials.empty()) materials.emplace_back();

	try {
		processNode(scene->mRootNode, scene);
	}
	catch (...) {
		for (const auto& texture : textures_loaded) glDeleteTextures(1, &texture.id);
		throw;
	}
}

Model::~Model() {
    for (const auto& texture : textures_loaded) glDeleteTextures(1, &texture.id);
}

void Model::draw(Shader& shader, const std::vector<Material>& overrides) {
	for (auto& mesh : meshes) {
		const auto index = mesh.materialIndex;
		const auto& material = index < overrides.size() ? overrides[index] : materials[index];
		mesh.draw(shader, material);
	}
};

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = mesh->HasNormals()
            ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
            : glm::vec3(0.0f, 1.0f, 0.0f);
		if (mesh->mTextureCoords[0]) {
			vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex < scene->mNumMaterials) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		auto appendMaps = [&](aiTextureType type, const char* usage) {
			auto maps = loadMaterialTextures(material, type, usage);
			textures.insert(textures.end(), maps.begin(), maps.end());
		};

		appendMaps(aiTextureType_DIFFUSE, "texture_diffuse");
		appendMaps(aiTextureType_NORMALS, "texture_normal");
		appendMaps(aiTextureType_HEIGHT, "texture_bump");
		appendMaps(aiTextureType_SPECULAR, "texture_specular");
		appendMaps(aiTextureType_OPACITY, "texture_opacity");
	}

	Mesh result(vertices, indices, textures);
	result.materialIndex = mesh->mMaterialIndex < materials.size() ? mesh->mMaterialIndex : 0;

	return result;
};

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	const bool srgb = type == aiTextureType_DIFFUSE;
	for (unsigned int i = 0; i < std::min(mat->GetTextureCount(type), 1u); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (textures_loaded[j].path == str.C_Str() && textures_loaded[j].srgb == srgb) {
				auto texture = textures_loaded[j];
				texture.type = typeName;
				textures.push_back(texture);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory, srgb);
			texture.type = typeName;
			texture.path = str.C_Str();
			texture.srgb = srgb;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
};

}
