#pragma once
#include <cineris/math/GLM.h>

namespace cineris {

struct Transform {
	glm::vec3 position{0.0f};
	// Euler angles in degrees, applied X then Y then Z.
	glm::vec3 rotation{0.0f};
	glm::vec3 scale{1.0f};

	glm::mat4 matrix() const {
		glm::mat4 result = glm::translate(glm::mat4(1.0f), position);
		result = glm::rotate(result, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		result = glm::rotate(result, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		result = glm::rotate(result, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		return glm::scale(result, scale);
	}
};
}
