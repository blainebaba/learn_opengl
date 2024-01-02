#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectMat;

void main()
{
    gl_Position = projectMat * viewMat * modelMat * vec4(aPos, 1.0);
    ourColor = aColor;
    texCoord = aTexCoord;
}
