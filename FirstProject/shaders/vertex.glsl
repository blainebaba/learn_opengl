#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec3 ourColor;
out vec2 texCoord;
out vec3 normal;
out vec3 modelPos;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectMat;

void main()
{
    modelPos = (modelMat * vec4(aPos, 1)).xyz;
    gl_Position = projectMat * viewMat * vec4(modelPos, 1.0);
    ourColor = aColor;
    texCoord = aTexCoord;
    // normal needs to transform to world coordinate
    normal = (modelMat * vec4(aNormal, 0)).xyz;
}
