#include <cineris/Application.h>
#include <cineris/ui/DebugUI.h>

namespace cineris {
Application::Application(const ApplicationSettings& settings)
    : m_window(settings.width, settings.height, settings.title) {
    DebugUI::Init(m_window.handle());
}
Application::~Application() { DebugUI::Shutdown(); }
}
