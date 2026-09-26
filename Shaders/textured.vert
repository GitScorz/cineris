#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec2 TexCoords;
out vec3 WorldPosition;
out vec3 WorldNormal;
out vec4 LightPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightMatrix;

void main() {
	TexCoords = aTexCoords;
	vec4 world = model * vec4(aPos, 1.0);
	WorldPosition = world.xyz;
	WorldNormal = transpose(inverse(mat3(model))) * aNormal;
	LightPosition = lightMatrix * world;
	gl_Position = projection * view * world;
}
