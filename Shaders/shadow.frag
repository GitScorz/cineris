#version 460 core

in vec2 TexCoords;

uniform sampler2D diffuseTexture;
uniform bool hasDiffuseTexture;
uniform sampler2D opacityTexture;
uniform bool hasOpacityTexture;
uniform float surfaceOpacity;
uniform float alphaCutoff;

void main() {
	float alpha = hasDiffuseTexture ? texture(diffuseTexture, TexCoords).a : 1.0;
	if (hasOpacityTexture) alpha *= texture(opacityTexture, TexCoords).r;
	if (alpha * surfaceOpacity < alphaCutoff) discard;
}
