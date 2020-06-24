#version 330 core
out vec4 FragColor;

in vec3 Unit;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

    vec3 normUnit = normalize(Unit);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(normUnit, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}