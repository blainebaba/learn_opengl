#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 modelMat;
uniform mat4 projectMat;
uniform mat4 viewMat;

out vec3 norm;
out vec2 texCoord;
out vec4 projPos;

void main() {
	projPos = projectMat * viewMat * modelMat * vec4(aPos, 1);
	gl_Position = projPos;
	norm = aNorm;
	texCoord = aTexCoord;
}