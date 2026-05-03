#version 330 core
out vec4 fragmentColor;

in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform int lightCount;
uniform vec3 lightPositions[8];

void main() {
  float ambientStrength = 0.15;
  vec3 ambient = ambientStrength * lightColor;

  vec3 diffuseSum = vec3(0.0);
  vec3 specularSum = vec3(0.0);

  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  for (int i = 0; i < lightCount; ++i) {
    vec3 lightDir = normalize(lightPositions[i] - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    diffuseSum += diff * lightColor;

    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    specularSum += specularStrength * spec * lightColor;
  }

  vec3 result = (ambient + diffuseSum + specularSum) * objectColor;
  fragmentColor = vec4(result, 1.0);
} 