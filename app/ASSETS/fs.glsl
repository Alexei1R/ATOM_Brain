#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 Texcordinates;
in vec3 FragPos;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;


vec3 lightPosition = vec3(0.0, 0.0,100.0);


void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 lightDir = normalize(lightPosition - FragPos);

    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * texture(texture_diffuse1, Texcordinates).xyz * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * texture(texture_specular1, Texcordinates).xyz * lightColor;

    // Ambient lighting
    float ambientStrength = 0.3; // You can adjust this according to your needs
    vec3 ambient = ambientStrength * texture(texture_diffuse1, Texcordinates).xyz * lightColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
