#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 lightPos;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light; 
uniform vec3 objectColor;
uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 Unit;
in vec3 FragPos;



void main()
{
    //ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));


    //diffuse
    vec3 normUnit = normalize(Unit);
    vec3 lightDir = normalize(light.lightPos - FragPos);

    float intense = max(dot(normUnit, lightDir), 0.0);
    vec3 diffuse = light.diffuse * intense * vec3(texture(material.diffuse, TexCoords));

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normUnit);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}