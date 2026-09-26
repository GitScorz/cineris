#include <cineris/renderer/Renderer.h>
#include <cineris/renderer/Model.h>
#include <glad/glad.h>
#include <cineris/scene/Scene.h>
#include <algorithm>
#include <stdexcept>

namespace cineris {

Renderer::Renderer() {
	glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer() {
	releaseTargets();
	glDeleteFramebuffers(1, &m_shadowFbo);
	glDeleteTextures(1, &m_shadowDepth);
	glDeleteVertexArrays(1, &m_screenVao);
}

void Renderer::configure(std::shared_ptr<Shader> lighting,
	std::shared_ptr<Shader> shadow,
	std::shared_ptr<Shader> tonemap) {
	if (!lighting || !shadow || !tonemap)
		throw std::invalid_argument("Renderer requires lighting, shadow and tonemap shaders.");

	m_lighting = std::move(lighting);
	m_shadow = std::move(shadow);
	m_tonemap = std::move(tonemap);
	if (m_shadowFbo) return;

	glGenVertexArrays(1, &m_screenVao);
	glGenFramebuffers(1, &m_shadowFbo);
	glGenTextures(1, &m_shadowDepth);

	glBindTexture(GL_TEXTURE_2D, m_shadowDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, ShadowResolution,
		ShadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	const float border[] = {1, 1, 1, 1};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowDepth, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Cannot create shadow framebuffer.");
}

void Renderer::releaseTargets() {
	glDeleteFramebuffers(1, &m_hdrFbo);
	glDeleteTextures(1, &m_hdrColor);
	glDeleteRenderbuffers(1, &m_hdrDepth);
	m_hdrFbo = m_hdrColor = m_hdrDepth = 0;
	m_width = m_height = 0;
}

void Renderer::resize(int width, int height) {
	if (width == m_width && height == m_height) return;

	releaseTargets();
	glGenFramebuffers(1, &m_hdrFbo);
	glGenTextures(1, &m_hdrColor);
	glBindTexture(GL_TEXTURE_2D, m_hdrColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenRenderbuffers(1, &m_hdrDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_hdrDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hdrColor, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hdrDepth);
	const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		releaseTargets();
		throw std::runtime_error("Cannot create HDR framebuffer.");
	}
	m_width = width;
	m_height = height;
}

void Renderer::render(const Scene& scene, const Camera& camera, int width, int height) {
	if (width <= 0 || height <= 0) return;
	if (!m_lighting) throw std::logic_error("Configure the renderer before rendering a scene.");

	resize(width, height);

	const auto& env = scene.environment;
	glm::vec3 direction = scene.sun.direction;
	direction = glm::length(direction) > 0.001f ? glm::normalize(direction) : glm::vec3(0, -1, 0);
	const float extent = std::max(env.shadowExtent, 1.0f);
	const auto up = std::abs(direction.y) > 0.99f ? glm::vec3(0, 0, 1) : glm::vec3(0, 1, 0);
	const auto lightMatrix = glm::ortho(-extent, extent, -extent, extent, 0.1f, extent * 4.0f)
		* glm::lookAt(env.shadowCenter - direction * extent * 2.0f, env.shadowCenter, up);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_FRAMEBUFFER_SRGB); // Tonemap pass explicitly encodes the final sRGB output.

	if (env.shadowsEnabled) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFbo);
		glViewport(0, 0, ShadowResolution, ShadowResolution);
		glClear(GL_DEPTH_BUFFER_BIT);
		m_shadow->use();
		m_shadow->setMat4("lightMatrix", lightMatrix);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(2.0f, 4.0f);

		for (const auto& object : scene.objects) {
			if (!object.visible || !object.castsShadow || !object.model) continue;
			if (glm::abs(object.transform.scale.x * object.transform.scale.y * object.transform.scale.z) < 1e-12f) continue;
			m_shadow->setMat4("model", object.transform.matrix());
			object.model->draw(*m_shadow, object.materials);
		}
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFbo);
	glViewport(0, 0, width, height);
	glClearColor(env.fogColor.r, env.fogColor.g, env.fogColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto& shader = *m_lighting;
	shader.use();
	shader.setMat4("view", camera.getViewMatrix());
	shader.setMat4("projection", camera.getProjectionMatrix());
	shader.setMat4("lightMatrix", lightMatrix);
	shader.setVec3("cameraPosition", camera.getPosition());
	shader.setVec3("sunDirection", direction);
	shader.setVec3("sunColor", scene.sun.color * std::max(scene.sun.intensity, 0.0f));
	shader.setVec3("ambientColor", env.ambient);
	shader.setVec3("fogColor", env.fogColor);
	shader.setFloat("fogDensity", env.fogEnabled ? std::max(env.fogDensity, 0.0f) : 0.0f);
	shader.setBool("shadowsEnabled", env.shadowsEnabled);
	shader.setInt("shadowMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_shadowDepth);
	glActiveTexture(GL_TEXTURE0);

	const int lightCount = static_cast<int>(std::min(scene.lights.size(), size_t(8)));
	shader.setInt("lightCount", lightCount);

	for (int i = 0; i < lightCount; ++i) {
		const auto prefix = "lights[" + std::to_string(i) + "].";
		shader.setVec3(prefix + "position", scene.lights[i].position);
		shader.setVec3(prefix + "color", scene.lights[i].color * std::max(scene.lights[i].intensity, 0.0f));
		shader.setFloat(prefix + "range", std::max(scene.lights[i].range, 0.01f));
	}

	for (const auto& object : scene.objects) {
		if (!object.visible || !object.model) continue;
		if (glm::abs(object.transform.scale.x * object.transform.scale.y * object.transform.scale.z) < 1e-12f) continue;
		shader.setMat4("model", object.transform.matrix());
		shader.setVec3("materialTint", object.tint);
		object.model->draw(shader, object.materials);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	m_tonemap->use();
	m_tonemap->setInt("hdrTexture", 0);
	m_tonemap->setFloat("exposure", std::max(env.exposure, 0.01f));
	m_tonemap->setFloat("saturation", std::clamp(env.saturation, 0.0f, 2.0f));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_hdrColor);
	glBindVertexArray(m_screenVao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::beginFrame() {
	glClearColor(0.025f, 0.025f, 0.035f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(Model& model, Shader& shader, const Camera& camera,
	const glm::mat4& transform) {
	shader.use();
	shader.setMat4("model", transform);
	shader.setMat4("view", camera.getViewMatrix());
	shader.setMat4("projection", camera.getProjectionMatrix());
	model.draw(shader);
}

void Renderer::endFrame() {}
}
