#pragma once
#include <cineris/scene/Transform.h>
#include <cineris/renderer/Material.h>
#include <memory>
#include <string>
#include <vector>

namespace cineris {

class Model;

struct SceneObject {
	std::string name;
	std::shared_ptr<Model> model;
	Transform transform;
	glm::vec3 tint{1.0f};

	// Empty slots use the model's imported defaults
	// Copies belong to this instance
	std::vector<Material> materials;
	bool visible = true;
	bool castsShadow = true;
};

struct DirectionalLight {
	glm::vec3 direction{-0.5f, -1.0f, -0.25f};
	glm::vec3 color{0.75f, 0.83f, 1.0f};
	float intensity = 1.6f;
};

struct PointLight {
	glm::vec3 position{0.0f};
	glm::vec3 color{1.0f, 0.65f, 0.35f};
	float intensity = 12.0f;
	float range = 8.0f;
};

struct Environment {
	glm::vec3 ambient{0.10f, 0.12f, 0.14f};
	glm::vec3 fogColor{0.20f, 0.24f, 0.25f};
	float fogDensity = 0.018f;
	float exposure = 1.2f;
	float saturation = 0.75f;
	bool fogEnabled = true;
	bool shadowsEnabled = true;
	glm::vec3 shadowCenter{0.0f, 3.0f, 0.0f};
	float shadowExtent = 25.0f;
};

struct Scene {
	std::vector<SceneObject> objects;
	DirectionalLight sun;
	std::vector<PointLight> lights;
	Environment environment;
};
}
