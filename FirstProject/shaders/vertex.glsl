#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 TexCoords;

uniform mat4 viewMat;
uniform mat4 projectMat;
uniform vec3 viewPos;

void main()
{
    gl_Position = projectMat * viewMat * vec4(aPos, 1.0);
    
    vec3 viewVec = normalize(aPos - viewPos);
    TexCoords = refract(viewVec, normalize(aNormal), 0.8);
}