#version 330 core

in vec4 shadowPos;
in vec3 norm;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 color;
uniform vec3 lightDi;

uniform sampler2D shadowTexture;

float computeShadow() {
	vec3 lightVec = normalize(-lightDi);
	vec3 normVec = normalize(norm);

	vec3 coord = shadowPos.xyz / shadowPos.w;
	coord = coord * 0.5 + 0.5;

	if (coord.z > 1)
		return 0;

	vec2 texelSize = 1.0/textureSize(shadowTexture,0);
	float curDepth = coord.z;
	float bias = (1-clamp(dot(lightVec,normVec),0,1))*0.001;

	float shadow = 0;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <=1; y++) {
			float depthInMap = texture(shadowTexture, coord.xy).r;
		    shadow += curDepth - depthInMap > bias ? 1.0 : 0.0;
		}
	}
	return shadow / 9.0;
}

void main() {
	vec3 lightVec = normalize(-lightDi);
	vec3 normVec = normalize(norm);

	float diffuse = clamp(dot(lightVec,normVec),0,1);

	// shadow
	float shadow = computeShadow();

	FragColor = vec4(color,1) * (diffuse * 0.5 * (1-shadow) + 0.2);
}