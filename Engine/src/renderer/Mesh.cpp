#include <cineris/renderer/Mesh.h>
#include <glad/glad.h>
#include <utility>

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
      VBO(std::exchange(other.VBO, 0)), EBO(std::exchange(other.EBO, 0)) {}

void Mesh::draw(Shader& shader) {
	shader.use();
	bool hasDiffuseTexture = false;
	for (const auto& texture : textures) {
		if (texture.type == "texture_diffuse" && texture.id != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture.id);
			shader.setInt("diffuseTexture", 0);
			hasDiffuseTexture = true;
			break;
		}
	}
	shader.setBool("hasDiffuseTexture", hasDiffuseTexture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

}
