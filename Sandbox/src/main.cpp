#include <Windows.h>
#include "Sandbox.h"
#include <filesystem>
#include <iostream>

int main() {
    try {
        // Assets are deployed beside the executable, independent of the working directory.
        wchar_t executable[32768];
        const DWORD length = GetModuleFileNameW(nullptr, executable, 32768);
        if (length == 0 || length == 32768)
            throw std::runtime_error("Cannot locate the Sandbox executable.");
        std::filesystem::current_path(std::filesystem::path(executable).parent_path());

        cineris::Application app({.title = "Cineris Sandbox"});
        Sandbox sandbox(app.window(), app.renderer(), {
            .position = glm::vec3(12.221539f, 1.332057f, -0.390347f)
        });
        double lastFrame = glfwGetTime();
        while (app.window().open()) {
            const double now = glfwGetTime();
            const float deltaTime = static_cast<float>(now - lastFrame);
            lastFrame = now;
            app.window().pollEvents();
            sandbox.processInput(deltaTime);
            sandbox.beginFrame();
            sandbox.endFrame();
            app.window().update();
        }
        std::cout << "Cineris Sandbox closed successfully.\n";
    } catch (const std::exception& error) {
        std::cerr << "Cineris: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
