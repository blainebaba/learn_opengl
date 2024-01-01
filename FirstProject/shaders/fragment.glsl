#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D container;
uniform sampler2D awesomeface;
uniform float smileLevel;

void main()
{
    FragColor = mix(texture(container, texCoord), texture(awesomeface, texCoord), smileLevel);
    // FragColor = vec4(ourColor, 1.0f);
}