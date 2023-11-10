#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

const vec3 lightDirection = vec3(1.0, -1.0, 0.0); // Direction of the light source
const vec3 lightColor = vec3(1.0, 1.0, 1.0);      // Color of the light
const vec3 objectColor = vec3(1.0, 0.0, 0.0);     // Color of the object material

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-lightDirection);

    float diffuseFactor = max(dot(norm, lightDir), 0.0);

    vec3 diffuseColor = lightColor * objectColor * diffuseFactor;

    FragColor = vec4(diffuseColor, 1.0);
}
