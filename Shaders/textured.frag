#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D diffuseTexture;
uniform bool hasDiffuseTexture;

void main() {
	FragColor = hasDiffuseTexture ? texture(diffuseTexture, TexCoords) : vec4(0.8, 0.8, 0.8, 1.0);
}
