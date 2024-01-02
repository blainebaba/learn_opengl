#version 330 core

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};

struct SpotLight {
    vec3 lightPos;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;
in vec3 normal;
in vec3 modelPos;

uniform Material material;
uniform SpotLight light;
uniform vec3 viewPos;

void main()
{

    vec3 lightVec = normalize(light.lightPos - modelPos);
    vec3 viewVec = normalize(viewPos - modelPos);
    vec3 reflectVec = reflect(-lightVec, normal);

    float diffuseAngleAttenuation = max(dot(lightVec,normal),0);
    float specularAngleAttenuation = pow(max(dot(reflectVec,viewVec),0), material.shininess);

    float dist = length(lightVec);
    float distanceAttenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    FragColor = texture(material.diffuseMap, texCoord) * (diffuseAngleAttenuation * distanceAttenuation * 2 + 0.03)
                + texture(material.specularMap, texCoord) * specularAngleAttenuation * distanceAttenuation * 4;
}