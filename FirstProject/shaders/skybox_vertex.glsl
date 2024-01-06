#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 viewMat;
uniform mat4 projectMat;

out vec3 texCoord;

void main() {
    vec4 pos = projectMat * viewMat * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
    texCoord = aPos;
}
