#include "Sandbox.h"
#include <cineris/ui/DebugUI.h>
using namespace cineris;

Sandbox::Sandbox(Window& window, Renderer& renderer, const CameraSettings& cameraSettings)
	: m_window(window), m_renderer(renderer)
{
	m_renderer.configure(
		m_assets.loadShader("Shaders/textured.vert", "Shaders/textured.frag"),
		m_assets.loadShader("Shaders/shadow.vert", "Shaders/shadow.frag"),
		m_assets.loadShader("Shaders/tonemap.vert", "Shaders/tonemap.frag")
	);

	auto sponza = m_assets.loadModel("Assets/sponza/sponza.obj");
	auto character = m_assets.loadModel("Assets/characters/daniel_holloway/daniel_holloway.obj");

	SceneObject architecture;
	architecture.name = "Sponza";
	architecture.model = sponza;
	architecture.materials = sponza->getMaterials();
	architecture.transform.scale = glm::vec3(0.01f);
	m_scene.objects.push_back(architecture);

	SceneObject person;
	person.name = "Character";
	person.model = character;
	person.materials = character->getMaterials();
	m_scene.objects.push_back(person);
	m_scene.lights.push_back({glm::vec3(4.0f, 2.0f, 0.0f)});
	m_scene.lights.push_back({glm::vec3(-4.0f, 2.0f, 0.0f)});

	m_previewScene.objects.push_back(person);
	person.name = "Shared model instance";
	person.transform.position.x = 1.2f;
	person.transform.rotation.y = -25.0f;
	person.tint = glm::vec3(0.7f, 0.85f, 1.0f);
	m_previewScene.objects.push_back(person);
	m_previewScene.environment.fogDensity = 0.025f;
	m_previewScene.environment.shadowCenter = glm::vec3(0.0f, 1.0f, 0.0f);
	m_previewScene.environment.shadowExtent = 4.0f;
	m_previewScene.lights.push_back({glm::vec3(-1.0f, 2.0f, 2.0f)});

	m_camera = std::make_unique<Camera>(
		cameraSettings.fov, 
		(float)window.getWidth() / (float)window.getHeight(), 
		0.05f, 
		100.0f, 
		cameraSettings.position
	);

	m_camera->setSensivity(cameraSettings.sensitivity);
	m_camera->setRotation(-3.0f, -180.0f);
}

void Sandbox::beginFrame() {
	DebugUI::StartFrame();
	if (m_renderingUI) drawSettings();

	if (m_window.getHeight() > 0) {
		m_camera->setAspectRatio(float(m_window.getWidth()) / float(m_window.getHeight()));
	}

	m_renderer.render(m_characterPreview ? m_previewScene : m_scene,
		*m_camera, m_window.getWidth(), m_window.getHeight());
}

void Sandbox::drawSettings() {
	auto& scene = m_characterPreview ? m_previewScene : m_scene;
	auto& environment = scene.environment;

	ImGui::Begin("Scene and atmosphere");
	ImGui::Text("F1: interface | F2: scene");
	ImGui::Text("Shared models: %zu", m_assets.modelCount());
	ImGui::Text("Camera: %.2f, %.2f, %.2f", m_camera->getPosition().x,
		m_camera->getPosition().y, m_camera->getPosition().z);

	if (ImGui::CollapsingHeader("Objects", ImGuiTreeNodeFlags_DefaultOpen)) {
		for (int i = 0; i < static_cast<int>(scene.objects.size()); ++i) {
			if (ImGui::Selectable(scene.objects[i].name.c_str(), m_selectedObject == i)) {
				m_selectedObject = i;
			}
		}

		auto& object = scene.objects[m_selectedObject];
		ImGui::Checkbox("Visible", &object.visible);
		ImGui::Checkbox("Casts shadow", &object.castsShadow);
		ImGui::DragFloat3("Position", &object.transform.position.x, 0.05f);
		ImGui::DragFloat3("Rotation", &object.transform.rotation.x, 0.5f);
		ImGui::DragFloat3("Scale", &object.transform.scale.x, 0.001f, 0.001f, 10.0f);
		ImGui::ColorEdit3("Tint", &object.tint.x);
	}

	if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_DefaultOpen)) {
		auto& object = scene.objects[m_selectedObject];
		ImGui::TextWrapped("Edits affect this object only. Values reset when the Sandbox closes.");

		for (size_t i = 0; i < object.materials.size(); ++i) {
			auto& material = object.materials[i];
			ImGui::PushID(static_cast<int>(i));

			if (ImGui::TreeNode("Material", "%s", material.name.c_str())) {
				ImGui::ColorEdit3("Base color", &material.tint.x);
				ImGui::SliderFloat("Roughness", &material.roughness, 0.05f, 1.0f);
				ImGui::SliderFloat("Specular", &material.specular, 0.0f, 1.0f);
				ImGui::SliderFloat("Normal strength", &material.normalStrength, 0.0f, 2.0f);
				ImGui::Checkbox("Flip normal green channel", &material.flipNormalY);
				ImGui::SliderFloat("Bump strength", &material.bumpStrength, 0.0f, 0.1f, "%.3f");
				ImGui::SliderFloat("Opacity mask", &material.opacity, 0.0f, 1.0f);
				ImGui::SliderFloat("Alpha cutoff", &material.alphaCutoff, 0.01f, 1.0f);
				ImGui::TextWrapped("Opacity uses cutouts, not blended glass.");

				if (ImGui::Button("Reset imported material")) {
					material = object.model->getMaterials()[i];
				}

				ImGui::TreePop();
			}

			ImGui::PopID();
		}
	}

	if (ImGui::CollapsingHeader("Lighting", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat3("Sun direction", &scene.sun.direction.x, 0.01f);
		ImGui::ColorEdit3("Sun color", &scene.sun.color.x);
		ImGui::SliderFloat("Sun intensity", &scene.sun.intensity, 0.0f, 5.0f);
		ImGui::ColorEdit3("Ambient", &environment.ambient.x);
		ImGui::Checkbox("Directional shadows", &environment.shadowsEnabled);
		ImGui::DragFloat3("Shadow center", &environment.shadowCenter.x, 0.1f);
		ImGui::SliderFloat("Shadow extent", &environment.shadowExtent, 1.0f, 60.0f);

		for (int i = 0; i < static_cast<int>(scene.lights.size()); ++i) {
			ImGui::PushID(i);
			if (ImGui::TreeNode("Point light", "Point light %d", i + 1)) {
				ImGui::DragFloat3("Position", &scene.lights[i].position.x, 0.05f);
				ImGui::ColorEdit3("Color", &scene.lights[i].color.x);
				ImGui::SliderFloat("Intensity", &scene.lights[i].intensity, 0.0f, 60.0f);
				ImGui::SliderFloat("Range", &scene.lights[i].range, 0.1f, 30.0f);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}

	if (ImGui::CollapsingHeader("Atmosphere", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Checkbox("Fog", &environment.fogEnabled);
		ImGui::ColorEdit3("Fog color", &environment.fogColor.x);
		ImGui::SliderFloat("Fog density", &environment.fogDensity, 0.0f, 0.15f);
		ImGui::SliderFloat("Exposure", &environment.exposure, 0.1f, 4.0f);
		ImGui::SliderFloat("Saturation", &environment.saturation, 0.0f, 1.5f);
	}

	ImGui::End();
}

void Sandbox::endFrame() {
	DebugUI::EndFrame();
    m_renderer.endFrame();
}

void Sandbox::processInput(float deltaTime) {
	GLFWwindow* windowHandle = m_window.handle();
	if (m_window.input()->wasReleased(GLFW_KEY_F2)) {
		m_selectedObject = 0;
		m_characterPreview = !m_characterPreview;
		if (m_characterPreview) {
			m_savedCameraPosition = m_camera->getPosition();
			m_savedCameraRotation = m_camera->getRotation();
			m_camera->setPosition(glm::vec3(0.0f, 1.05f, 3.2f));
			m_camera->setRotation(-3.0f, -90.0f);
		}
		else {
			m_camera->setPosition(m_savedCameraPosition);
			m_camera->setRotation(m_savedCameraRotation);
		}
	}

	if (m_window.input()->wasReleased(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(windowHandle, true);
	}

	const float distance = 4.5f * deltaTime;

	if (!m_renderingUI) {
		if (m_window.input()->isPressed(GLFW_KEY_W)) m_camera->moveForward(distance);
		if (m_window.input()->isPressed(GLFW_KEY_S)) m_camera->moveForward(-distance);
		if (m_window.input()->isPressed(GLFW_KEY_D)) m_camera->moveRight(distance);
		if (m_window.input()->isPressed(GLFW_KEY_A)) m_camera->moveRight(-distance);

		if (m_window.input()->isPressed(GLFW_KEY_E)) m_camera->moveUp(distance);
		if (m_window.input()->isPressed(GLFW_KEY_Q)) m_camera->moveUp(-distance);
	}

	if (m_window.input()->wasReleased(GLFW_KEY_F1)) {
		m_renderingUI = !m_renderingUI;

		if (m_renderingUI) {
			m_camera->setMouseMovementEnabled(false);
			glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_camera->setMouseMovementEnabled(true);
			m_firstMouse = true;
		}
	}

	double mouseX, mouseY;
	glfwGetCursorPos(windowHandle, &mouseX, &mouseY);
	if (m_firstMouse) {
		m_lastMouseX = mouseX;
		m_lastMouseY = mouseY;
		m_firstMouse = false;
	}
	m_camera->processMouseMovement(static_cast<float>(mouseX - m_lastMouseX), static_cast<float>(m_lastMouseY - mouseY), true);
	m_lastMouseX = mouseX;
	m_lastMouseY = mouseY;

}
