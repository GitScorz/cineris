#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec3 vWorldPos;

void main() {
  float waveHeight = 0.2;
  float waveFrequency = 2.0;

  vec3 pos = aPos;
  pos.y += sin(pos.x * waveFrequency + time) * waveHeight;
  pos.y += cos(pos.z * waveFrequency + time) * waveHeight * 0.5;

  vWorldPos = pos;
  gl_Position = projection * view * model * vec4(pos, 1.0);
}