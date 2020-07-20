#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    float shininess;
};

struct Light{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct DirLight {
    vec3 direction;
};
struct PointLight {
    vec3 position;
  
    float constant;
    float linear;
    float quadratic;    
};
struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    
    float cutOff;
    float outerCutOff;
};

#define INDEX_POINT_LIGHT 4

uniform Material material;

uniform Light light;
uniform DirLight dirLight;
uniform PointLight pointLight[INDEX_POINT_LIGHT];
uniform SpotLight spotLight;


uniform vec3 viewPos;

vec3 CalcDirLight(DirLight dirLight, vec3 normUnit);
vec3 CalcPointLight(PointLight pointLight, vec3 normUnit);
vec3 CalcSpotLight(SpotLight spotLight, vec3 normUnit);

in vec2 TexCoords;
in vec3 Unit;
in vec3 FragPos;




void main()
{
    vec3 result = vec3(0.0);
    vec3 normUnit = normalize(Unit);
   
    if (dirLight.direction == vec3(-0.2f, -1.0f, -0.3f)) {
        result += CalcDirLight(dirLight, normUnit);
    }
   
    for (int i = 0; i < INDEX_POINT_LIGHT; i++) {
        if (pointLight[i].constant == 1.0) {
            result += CalcPointLight(pointLight[i], normUnit);
	    }
    }
    
    if (spotLight.constant == 1.0) {
        result += CalcSpotLight(spotLight, normUnit); 
    }
    
 
    FragColor = vec4(result, 1.0);
}



    vec3 CalcDirLight(DirLight dirLight, vec3 normUnit) {
    
        vec3 lightDir = normalize(-dirLight.direction);

        //ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuseTexture, TexCoords));

        //diffuse
        float intenseDiffuse = max(dot(normUnit, lightDir), 0.0);
        vec3 diffuse = light.diffuse * intenseDiffuse * vec3(texture(material.diffuseTexture, TexCoords));

        //specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normUnit);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specularTexture, TexCoords));

        return (ambient + diffuse + specular);
}



    vec3 CalcPointLight(PointLight pointLight, vec3 normUnit) {
    
        vec3 lightDir = normalize(pointLight.position - FragPos);

        //ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuseTexture, TexCoords));

        //diffuse
        float intenseDiffuse = max(dot(normUnit, lightDir), 0.0);
        vec3 diffuse = light.diffuse * intenseDiffuse * vec3(texture(material.diffuseTexture, TexCoords));

        //specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normUnit);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specularTexture, TexCoords));
    
        // attenuation
        float attenuationDist    = length(pointLight.position - FragPos);
        float intenseAttenuation =  1.0 / ( pointLight.constant +
                                    pointLight.linear * attenuationDist + 
                                    pointLight.quadratic * (attenuationDist * attenuationDist));    

        ambient  *= intenseAttenuation;  
        diffuse   *= intenseAttenuation;
        specular *= intenseAttenuation;   

        return (ambient + diffuse + specular);
}


    vec3 CalcSpotLight(SpotLight spotLight , vec3 normUnit) {
    
        vec3 lightDir = normalize(spotLight.position - FragPos);

        //ambient
	    vec3 ambient = spotLight.ambient * vec3(texture(material.diffuseTexture, TexCoords));

        //diffuse
        float intenseDiffuse = max(dot(normUnit, lightDir), 0.0);
        vec3 diffuse = spotLight.diffuse * intenseDiffuse * vec3(texture(material.diffuseTexture, TexCoords));

        //specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normUnit);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spotLight.specular * spec * vec3(texture(material.specularTexture, TexCoords));

        float angleToFragment = dot(lightDir, normalize(-spotLight.direction));
        float cutOffDifference = spotLight.cutOff - spotLight.outerCutOff;
        float intenseSpotLight = clamp((angleToFragment - spotLight.outerCutOff) / cutOffDifference, 0.0, 1.0);

        float attenuationDist = length(spotLight.position - FragPos);
        float intenseAttenuation = 1.0 / ( spotLight.constant +
                                spotLight.linear * attenuationDist + 
                                spotLight.quadratic * (attenuationDist * attenuationDist));

        ambient *= intenseAttenuation;
        diffuse *= intenseAttenuation * intenseSpotLight;
        specular *= intenseAttenuation * intenseSpotLight;

        return (ambient + diffuse + specular);
}
