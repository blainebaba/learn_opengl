#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;
in vec3 normal;
in vec3 modelPos;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};

uniform Material material;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // vec4 textureColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);

    vec3 lightVec = normalize(lightPos - modelPos);
    vec3 viewVec = normalize(viewPos - modelPos);
    vec3 reflectVec = reflect(-lightVec, normal);

    float diffuseLight = max(dot(lightVec,normal),0);
    float specularLight = pow(max(dot(reflectVec,viewVec),0), material.shininess);

    FragColor = texture(material.diffuseMap, texCoord) * diffuseLight
                + texture(material.specularMap, texCoord) * specularLight;
}