#version 330 core

out vec4 fragmentColor;
in vec3 vWorldPos;

uniform vec3 lightDir;

void main() {
  vec3 normal = normalize(vec3(0.0, 1.0, 0.0));

  float diff = max(dot(normal, normalize(-lightDir)), 0.0);

  vec3 waterColor = vec3(0.0, 0.3, 0.6);

  fragmentColor = vec4(waterColor * diff, 1.0);
}