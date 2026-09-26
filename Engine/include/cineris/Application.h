#pragma once
#include <cineris/Window.h>
#include <cineris/renderer/Renderer.h>

namespace cineris {
struct CameraSettings {
    float fov = 45.0f;
    float sensitivity = 0.1f;
    glm::vec3 position{0.0f};
};
struct ApplicationSettings {
    unsigned int width = 1280;
    unsigned int height = 720;
    std::string title = "Cineris";
};

// Declare this before application-owned GPU resources, so it outlives them.
class Application {
public:
    explicit Application(const ApplicationSettings& settings = {});
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Window& window() { return m_window; }
    Renderer& renderer() { return m_renderer; }
private:
    Window m_window;
    Renderer m_renderer;
};
}
