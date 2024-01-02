#version 330 core

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};

struct Light {
    vec3 lightPos;
    vec3 lightDir;

    float constant;
    float linear;
    float quadratic;

    float cutoff; // spot light
    float cutoffStart;
};

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;
in vec3 normal;
in vec3 modelPos;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    
    vec3 lightVec = normalize(light.lightPos - modelPos);
    float lightAngle = dot(-lightVec, light.lightDir);
    
    float ambientLight;
    float diffuseLight;
    float specularLight;

    if (lightAngle < light.cutoff) {
        ambientLight = 0.1;
        diffuseLight = 0;
        specularLight = 0;
    } else {
		vec3 viewVec = normalize(viewPos - modelPos);
		vec3 reflectVec = reflect(-lightVec, normal);

		float diffuseAngleAttenuation = max(dot(lightVec,normal),0);
		float specularAngleAttenuation = pow(max(dot(reflectVec,viewVec),0), material.shininess);

		float dist = length(lightVec);
		float distanceAttenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

        // smooth cutoff
        float smoothFactor =  clamp((lightAngle - light.cutoff) / (light.cutoffStart - light.cutoff), 0, 1);

        ambientLight = 0.1;
        diffuseLight = diffuseAngleAttenuation * distanceAttenuation * smoothFactor * 2;
        specularLight = specularAngleAttenuation * distanceAttenuation * smoothFactor * 2;
    }



    FragColor = texture(material.diffuseMap, texCoord) * (diffuseLight + ambientLight)
                + texture(material.specularMap, texCoord) * specularLight; 
}