#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <cineris/renderer/Shader.h>
#include <cineris/renderer/Material.h>

namespace cineris {

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
	bool srgb = false;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int materialIndex = 0;

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	~Mesh();
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&&) = delete;

	void draw(Shader& shader, const Material& material);

private:
	unsigned int VAO = 0, VBO = 0, EBO = 0;

	void setupMesh();

};
}
