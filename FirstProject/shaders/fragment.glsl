#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 modelPos;
in vec3 normal;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;

void main()
{    
    vec3 texColor = texture(texture_diffuse1, TexCoords).xyz;

    vec3 lightVec = normalize(lightPos - modelPos);
    vec3 normNorm = normalize(normal);

    float diffuse = dot(lightVec, normNorm);

    FragColor = vec4(texColor * (diffuse + 0.1), 1);
}