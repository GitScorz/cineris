#include <cineris/renderer/Model.h>
#include <iostream>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

namespace cineris {

unsigned int TextureFromFile(const char* path, const std::string& directory) {
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;

	// Assimp already flips the UV coordinates during import.

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format = GL_RGBA;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 2)
			format = GL_RG;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		GLint alignment;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else {
		std::cerr << "Texture failed to load at path: " << filename << std::endl;
		stbi_image_free(data);
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
	return textureID;
}

Model::Model(const std::string& filepath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error("Cannot load model " + filepath + ": " + importer.GetErrorString());
	}

	directory = filepath.substr(0, filepath.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

Model::~Model() {
    for (const auto& texture : textures_loaded) glDeleteTextures(1, &texture.id);
}

void Model::draw(Shader& shader) {
	for (auto& mesh : meshes) {
		mesh.draw(shader);
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

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
};

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
};

}
