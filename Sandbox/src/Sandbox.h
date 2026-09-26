#pragma once
#include <cineris/Application.h>
#include <cineris/renderer/Model.h>
#include <cineris/renderer/Shader.h>
#include <memory>
class Sandbox {
public:
	Sandbox(cineris::Window& window, cineris::Renderer& renderer, const cineris::CameraSettings& cameraSettings);

	void beginFrame();
	void endFrame();
	void processInput(float deltaTime);

private:
	cineris::Window& m_window;
    cineris::Renderer& m_renderer;

	std::unique_ptr<cineris::Shader> m_sponzaShader;
	std::unique_ptr<cineris::Model> m_sponzaModel;
	std::unique_ptr<cineris::Model> m_characterModel;

	std::unique_ptr<cineris::Camera> m_camera;
	bool m_firstMouse = true;
	double m_lastMouseX = 0.0;
	double m_lastMouseY = 0.0;

	bool m_renderingUI = false;
	bool m_characterPreview = false;
	glm::vec3 m_savedCameraPosition = glm::vec3(0.0f);
	glm::vec3 m_savedCameraRotation = glm::vec3(0.0f);
};
