#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform bool depthMap;

void main() {
	float depth = texture(texture1, texCoord).x;
	FragColor = vec4(vec3(depth), 1);
}