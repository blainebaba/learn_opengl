#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 modelMat;
uniform mat4 projectMat;
uniform mat4 viewMat;
uniform mat4 shadowViewMat;
uniform mat4 shadowProjectMat;

out vec4 shadowPos;
out vec3 norm;
out vec2 texCoord;

void main() {
	vec4 modelPos = modelMat * vec4(aPos,1);
	gl_Position = projectMat * viewMat * modelPos;
	shadowPos = shadowProjectMat * shadowViewMat * modelPos;
	norm = aNorm;
	texCoord = aTexCoord;
}