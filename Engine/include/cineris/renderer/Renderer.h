#pragma once
#include <cineris/camera/Camera.h>
#include <memory>

namespace cineris {
class Model;
class Shader;
struct Scene;

// OpenGL implementation; the caller owns scene content and camera controls.
class Renderer {
public:
	Renderer();
	~Renderer();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	void configure(std::shared_ptr<Shader> lighting, std::shared_ptr<Shader> shadow,
				   std::shared_ptr<Shader> tonemap);
	void render(const Scene& scene, const Camera& camera, int width, int height);
	void beginFrame();
	void draw(Model& model, Shader& shader, const Camera& camera,
			  const glm::mat4& transform = glm::mat4(1.0f));
	void endFrame();
private:
	void resize(int width, int height);
	void releaseTargets();
	std::shared_ptr<Shader> m_lighting, m_shadow, m_tonemap;
	unsigned int m_hdrFbo = 0, m_hdrColor = 0, m_hdrDepth = 0;
	unsigned int m_shadowFbo = 0, m_shadowDepth = 0, m_screenVao = 0;
	int m_width = 0, m_height = 0;
	static constexpr int ShadowResolution = 2048;
};
}
