#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform float waveHeight;
uniform float waveFrequency;

out vec3 vWorldPos;

void main() {
  vec3 pos = aPos;
  pos.y += sin(pos.x * waveFrequency + time) * waveHeight;
  pos.y += cos(pos.z * waveFrequency + time) * waveHeight * 0.5;

  vWorldPos = pos;
  gl_Position = projection * view * model * vec4(pos, 1.0);
}
