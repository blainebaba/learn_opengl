#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;
in vec3 normal;
in vec3 modelPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec4 textureColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);

    vec3 lightVec = normalize(lightPos - modelPos);
    vec3 viewVec = normalize(viewPos - modelPos);

    float diffuseLight = max(dot(lightVec,normal),0);
    vec3 reflectVec = reflect(-lightVec, normal);
    float specularLight = pow(max(dot(reflectVec,viewVec),0), 25);

    float lightStrengh = 0.05 + diffuseLight * 0.5 + specularLight * 0.7;

    // FragColor = textureColor * 0.2 + specularLight;
    // FragColor = vec4(normal, 1);
    // FragColor = textureColor;
    FragColor = vec4(1) * lightStrengh ;
}