#include <cineris/renderer/Mesh.h>
#include <glad/glad.h>
#include <utility>
#include <algorithm>

namespace cineris {

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
	: vertices(vertices), indices(indices), textures(textures) {
	setupMesh();
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	//pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	//tex coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

Mesh::Mesh(Mesh&& other) noexcept
    : vertices(std::move(other.vertices)), indices(std::move(other.indices)),
      textures(std::move(other.textures)), VAO(std::exchange(other.VAO, 0)),
      VBO(std::exchange(other.VBO, 0)), EBO(std::exchange(other.EBO, 0)) {
	materialIndex = other.materialIndex;
}

void Mesh::draw(Shader& shader, const Material& material) {
	shader.use();
	shader.setVec3("surfaceColor", material.tint);
	shader.setFloat("surfaceOpacity", std::clamp(material.opacity, 0.0f, 1.0f));
	shader.setFloat("roughness", std::clamp(material.roughness, 0.05f, 1.0f));
	shader.setFloat("specularStrength", std::clamp(material.specular, 0.0f, 1.0f));
	shader.setFloat("alphaCutoff", std::clamp(material.alphaCutoff, 0.0f, 1.0f));
	shader.setFloat("normalStrength", std::max(material.normalStrength, 0.0f));
	shader.setFloat("bumpStrength", std::max(material.bumpStrength, 0.0f));
	shader.setBool("flipNormalY", material.flipNormalY);

	auto bindTexture = [&](const char* type, const char* sampler, const char* flag, int unit) {
		unsigned int id = 0;
		for (const auto& texture : textures) {
			if (texture.type == type && texture.id != 0) {
				id = texture.id;
				break;
			}
		}

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, id);
		shader.setInt(sampler, unit);
		shader.setBool(flag, id != 0);
	};

	// Unit 1 belongs to the renderer's shadow map
	bindTexture("texture_diffuse", "diffuseTexture", "hasDiffuseTexture", 0);
	bindTexture("texture_normal", "normalTexture", "hasNormalTexture", 2);
	bindTexture("texture_specular", "specularTexture", "hasSpecularTexture", 3);
	bindTexture("texture_opacity", "opacityTexture", "hasOpacityTexture", 4);
	bindTexture("texture_bump", "bumpTexture", "hasBumpTexture", 5);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

}
