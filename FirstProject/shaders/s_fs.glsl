#version 330 core

in vec3 norm;
in vec2 texCoord;
in vec4 projPos;

out vec4 FragColor;

uniform vec3 color;
uniform vec3 lightDi;

void main() {
	vec3 normPos = projPos.xyz/projPos.w;
	normPos = normPos  * 0.5 + 0.5;

	FragColor = vec4(normPos.z, 0, 0,1);
}