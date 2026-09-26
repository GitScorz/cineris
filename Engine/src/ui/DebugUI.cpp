#include <cineris/ui/DebugUI.h>

namespace cineris {

namespace DebugUI {

	void Init(GLFWwindow* window) {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = NULL;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void StartFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EndFrame() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	//void Draw(bool render) {
	//	if (!render) return;

	//	ImGui::Begin("Debug UI");
	//	ImGui::End();
	//}

}
}
