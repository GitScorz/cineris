#version 460 core

out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPosition;
in vec3 WorldNormal;
in vec4 LightPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform sampler2D normalTexture;
uniform sampler2D bumpTexture;
uniform sampler2D specularTexture;
uniform sampler2D opacityTexture;

uniform bool hasDiffuseTexture;
uniform bool shadowsEnabled;
uniform bool hasNormalTexture;
uniform bool hasBumpTexture;
uniform bool hasSpecularTexture;
uniform bool hasOpacityTexture;
uniform bool flipNormalY;

uniform vec3 surfaceColor;
uniform vec3 materialTint;
uniform vec3 cameraPosition;
uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform vec3 ambientColor;
uniform vec3 fogColor;

uniform float surfaceOpacity;
uniform float roughness;
uniform float specularStrength;
uniform float alphaCutoff;
uniform float fogDensity;
uniform float normalStrength;
uniform float bumpStrength;

struct PointLight {
	vec3 position;
	vec3 color;
	float range;
};

uniform int lightCount;
uniform PointLight lights[8];

float shadowVisibility(vec3 normal, vec3 lightDirection) {
	if (!shadowsEnabled) return 1.0;

	vec3 projected = LightPosition.xyz / LightPosition.w * 0.5 + 0.5;
	if (projected.z <= 0.0 || projected.z >= 1.0 ||
		any(lessThan(projected.xy, vec2(0.0))) || any(greaterThan(projected.xy, vec2(1.0)))) return 1.0;

	float bias = max(0.0008 * (1.0 - dot(normal, lightDirection)), 0.00015);
	vec2 texel = 1.0 / vec2(textureSize(shadowMap, 0));
	float visibility = 0.0;

	for (int y = -1; y <= 1; ++y) {
		for (int x = -1; x <= 1; ++x) {
			float depth = texture(shadowMap, projected.xy + vec2(x, y) * texel).r;
			visibility += projected.z - bias <= depth ? 1.0 : 0.0;
		}
	}

	return visibility / 9.0;
}

vec3 surfaceNormal() {
	vec3 normal = normalize(WorldNormal);
	if (!gl_FrontFacing) normal = -normal;

	vec3 dpdx = dFdx(WorldPosition);
	vec3 dpdy = dFdy(WorldPosition);
	vec2 duvdx = dFdx(TexCoords);
	vec2 duvdy = dFdy(TexCoords);
	float determinant = duvdx.x * duvdy.y - duvdx.y * duvdy.x;
	if (abs(determinant) < 0.00000001) return normal;

	vec3 tangent = (dpdx * duvdy.y - dpdy * duvdx.y) / determinant;
	vec3 bitangent = (-dpdx * duvdy.x + dpdy * duvdx.x) / determinant;
	tangent -= normal * dot(normal, tangent);
	if (dot(tangent, tangent) < 0.00000001) return normal;

	tangent = normalize(tangent);
	float handedness = dot(cross(normal, tangent), bitangent) < 0.0 ? -1.0 : 1.0;
	bitangent = cross(normal, tangent) * handedness;
	vec3 detail = vec3(0.0, 0.0, 1.0);

	if (hasNormalTexture) {
		detail = texture(normalTexture, TexCoords).xyz * 2.0 - 1.0;
		detail.xy *= normalStrength;
		if (flipNormalY) detail.y = -detail.y;
		detail.z = max(detail.z, 0.001);
	}
	else if (hasBumpTexture && bumpStrength > 0.0) {
		vec2 texel = 1.0 / vec2(textureSize(bumpTexture, 0));
		float left = texture(bumpTexture, TexCoords - vec2(texel.x, 0.0)).r;
		float right = texture(bumpTexture, TexCoords + vec2(texel.x, 0.0)).r;
		float down = texture(bumpTexture, TexCoords - vec2(0.0, texel.y)).r;
		float up = texture(bumpTexture, TexCoords + vec2(0.0, texel.y)).r;
		detail.xy = -vec2((right - left) / texel.x, (up - down) / texel.y) * 0.5 * bumpStrength;
	}

	return normalize(mat3(tangent, bitangent, normal) * normalize(detail));
}

vec3 illuminate(vec3 albedo, vec3 normal, vec3 viewDirection, vec3 direction, vec3 radiance, float specularAmount) {
	float diffuse = max(dot(normal, direction), 0.0);
	vec3 halfVector = direction + viewDirection;
	float exponent = max(2.0 / (roughness * roughness) - 2.0, 1.0);
	float specular = 0.0;

	if (diffuse > 0.0 && dot(halfVector, halfVector) > 0.00001) {
		specular = pow(max(dot(normal, normalize(halfVector)), 0.0), exponent) * specularAmount;
	}

	return (albedo * diffuse + vec3(specular)) * radiance;
}

void main() {
	// Color textures are decoded from sRGB by the GPU
	vec4 texel = hasDiffuseTexture ? texture(diffuseTexture, TexCoords) : vec4(1.0);
	float opacity = hasOpacityTexture ? texture(opacityTexture, TexCoords).r : 1.0;

	// Derivatives must be evaluated before discarding fragments in a quad
	vec3 normal = surfaceNormal();
	if (texel.a * surfaceOpacity * opacity < alphaCutoff) discard;

	vec3 albedo = texel.rgb * surfaceColor * materialTint;
	float specularAmount = specularStrength;
	if (hasSpecularTexture) specularAmount *= texture(specularTexture, TexCoords).r;

	vec3 viewDirection = normalize(cameraPosition - WorldPosition);
	vec3 lightDirection = -sunDirection;
	vec3 color = albedo * ambientColor;
	color += illuminate(albedo, normal, viewDirection, lightDirection, sunColor, specularAmount)
		* shadowVisibility(normalize(WorldNormal), lightDirection);

	for (int i = 0; i < lightCount; ++i) {
		vec3 offset = lights[i].position - WorldPosition;
		float distanceToLight = length(offset);
		float falloff = clamp(1.0 - pow(distanceToLight / lights[i].range, 4.0), 0.0, 1.0);
		falloff = falloff * falloff / (1.0 + distanceToLight * distanceToLight);
		color += illuminate(albedo, normal, viewDirection,
			offset / max(distanceToLight, 0.0001), lights[i].color * falloff, specularAmount);
	}

	float visibility = exp(-fogDensity * length(cameraPosition - WorldPosition));
	FragColor = vec4(mix(fogColor, color, visibility), 1.0);
}
