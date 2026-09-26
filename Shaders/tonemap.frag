#version 460 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D hdrTexture;
uniform float exposure;
uniform float saturation;

vec3 encodeSRGB(vec3 color) {
	return mix(12.92 * color, 1.055 * pow(color, vec3(1.0 / 2.4)) - 0.055,
		step(vec3(0.0031308), color));
}

void main() {
	vec3 color = max(texture(hdrTexture, TexCoords).rgb, vec3(0.0)) * exposure;
	// Reinhard compresses HDR highlights before display conversion
	color = color / (1.0 + color);
	float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));
	color = clamp(mix(vec3(luminance), color, saturation), 0.0, 1.0);
	FragColor = vec4(encodeSRGB(color), 1.0);
}
