#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
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

#define INDEX_POINT_LIGHT 1

uniform Material material;

uniform Light light;
uniform PointLight pointLight[INDEX_POINT_LIGHT];

uniform vec3 viewPos;

vec3 CalcPointLight(PointLight pointLight, vec3 normNormal);

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;


void main()
{   
    vec3 result = vec3(0.0);
    vec3 normNormal = normalize(Normal);

    for (int i = 0; i < INDEX_POINT_LIGHT; i++) {
        if (pointLight[i].constant == 1.0) {
            result += CalcPointLight(pointLight[i], normNormal);
	    }
    }

    FragColor = vec4(result, 1.0);

}

    vec3 CalcPointLight(PointLight pointLight, vec3 normNormal) {
    
        vec3 lightDir = normalize(pointLight.position - FragPos);

        //ambient
        vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

        //diffuse
        float intenseDiffuse = max(dot(normNormal, lightDir), 0.0);
        vec3 diffuse = light.diffuse * intenseDiffuse * vec3(texture(material.texture_diffuse1, TexCoords));

        //specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normNormal);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
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