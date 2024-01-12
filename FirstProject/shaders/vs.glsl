#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 projectMat;
uniform mat4 viewMat;

void main() {
	gl_Position = projectMat * viewMat * vec4(pos, 1);
}