#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3  direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    
    float cutOff;
    float outerCutOff;
};

uniform Material material;
uniform Light light; 
uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 Unit;
in vec3 FragPos;



void main()
{
    vec3 lightDir = normalize(light.position - FragPos);

    //ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    //diffuse
    vec3 normUnit = normalize(Unit);


    float intenseDiffuse = max(dot(normUnit, lightDir), 0.0);
    vec3 diffuse = light.diffuse * intenseDiffuse * vec3(texture(material.diffuse, TexCoords));

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normUnit);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    float angleToFragment = dot(lightDir, normalize(-light.direction));
    float cutOffDifference = light.cutOff - light.outerCutOff;
    float intenseSpotlight = clamp((angleToFragment - light.outerCutOff) / cutOffDifference, 0.0, 1.0);

    float attenuationDist = length(light.position - FragPos);
    float intenseAttenuation = 1.0 / ( light.constant +
                                light.linear * attenuationDist + 
                                light.quadratic * (attenuationDist * attenuationDist));

    ambient *= intenseAttenuation;
    diffuse *= intenseAttenuation * intenseSpotlight;
    specular *= intenseAttenuation * intenseSpotlight;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}