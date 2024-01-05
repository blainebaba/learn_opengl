#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 modelPos;
out vec3 normal;

uniform mat4 viewMat;
uniform mat4 projectMat;

void main()
{
    gl_Position = projectMat * viewMat * vec4(aPos, 1.0);
    TexCoords = aTexCoords;    
    modelPos = aPos;
    normal = aNormal;
}