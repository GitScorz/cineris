#include "Sandbox.h"
#include <cineris/ui/DebugUI.h>
using namespace cineris;

Sandbox::Sandbox(Window& window, Renderer& renderer, const CameraSettings& cameraSettings)
	: m_window(window), m_renderer(renderer)
{
	m_sponzaShader = std::make_unique<Shader>("Shaders/textured.vert", "Shaders/textured.frag", "");
	m_sponzaModel = std::make_unique<Model>("Assets/sponza/sponza.obj");
	m_characterModel = std::make_unique<Model>("Assets/characters/daniel_holloway/daniel_holloway.obj");

	m_camera = std::make_unique<Camera>(
		cameraSettings.fov, 
		(float)window.getWidth() / (float)window.getHeight(), 
		0.05f, 
		100.0f, 
		cameraSettings.position
	);

	m_camera->setSensivity(cameraSettings.sensitivity);




}

void Sandbox::beginFrame() {
	m_renderer.beginFrame();
    if (m_window.getHeight() > 0)
        m_camera->setAspectRatio(float(m_window.getWidth()) / float(m_window.getHeight()));
    m_renderer.draw(m_characterPreview ? *m_characterModel : *m_sponzaModel,
        *m_sponzaShader, *m_camera, m_characterPreview ? glm::mat4(1.0f)
        : glm::scale(glm::mat4(1.0f), glm::vec3(0.01f)));

	DebugUI::StartFrame();
	//DebugUI::Draw(m_renderingUI);

	if (m_renderingUI) {
		ImGui::Begin("Debug UI");
		ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", m_camera->getPosition().x, m_camera->getPosition().y, m_camera->getPosition().z);
		ImGui::SameLine();
		if (ImGui::Button("Copy")) {
			std::string text = std::to_string(m_camera->getPosition().x) + ", " + std::to_string(m_camera->getPosition().y) + ", " + std::to_string(m_camera->getPosition().z);
			ImGui::SetClipboardText(text.c_str());
		}

		ImGui::End();
	}
}

void Sandbox::endFrame() {
	DebugUI::EndFrame();
    m_renderer.endFrame();
}

void Sandbox::processInput(float deltaTime) {
	GLFWwindow* windowHandle = m_window.handle();
	if (m_window.input()->wasReleased(GLFW_KEY_F2)) {
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

	if (m_window.input()->isPressed(GLFW_KEY_W)) m_camera->moveForward(distance);
	if (m_window.input()->isPressed(GLFW_KEY_S)) m_camera->moveForward(-distance);
	if (m_window.input()->isPressed(GLFW_KEY_D)) m_camera->moveRight(distance);
	if (m_window.input()->isPressed(GLFW_KEY_A)) m_camera->moveRight(-distance);

	if (m_window.input()->isPressed(GLFW_KEY_E)) m_camera->moveUp(distance);
	if (m_window.input()->isPressed(GLFW_KEY_Q)) m_camera->moveUp(-distance);

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
