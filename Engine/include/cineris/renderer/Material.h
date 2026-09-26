#pragma once
#include <glm/glm.hpp>
#include <string>

namespace cineris {

struct Material {
	std::string name;
	glm::vec3 tint{1.0f};
	float opacity = 1.0f;
	float roughness = 0.8f;
	float specular = 0.15f;
	float alphaCutoff = 0.1f;
	float normalStrength = 1.0f;
	float bumpStrength = 0.005f;
	bool flipNormalY = false;
};
}
