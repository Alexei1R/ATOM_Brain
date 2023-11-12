#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;



//new stuff

out vec3 Normal;
out vec3 Position;
out vec3 Color;





out vec3 normal;
out vec2 Texcordinates;
out vec3 FragPos;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNorm;
    Position = vec3(model * vec4(aPos, 1.0));

    gl_Position = proj * view * model * vec4(aPos, 1.0);

    FragPos = vec3(model * vec4(aPos, 1.0));
    Texcordinates = aTex;
    normal = aNorm;
    Color = aColor;
}


