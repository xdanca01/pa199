#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 diffMat;
uniform vec3 specMat;
uniform vec3 ambiMat;

out vec2 tex_coord;
out vec3 normal_vec;
out vec3 lightCol;
out vec3 lightPos;
out vec3 FragPos;
out vec3 viewPos;
out vec3 tmp;
out vec3 ambientMat;
out vec3 diffuseMat;
out vec3 specularMat;

void main()
{
    //gl_Position = vec4(position, 1.0);
	gl_Position = projection * view * model * vec4(position, 1.0);
    tex_coord = texcoord;
    normal_vec = mat3(transpose(inverse(model))) * normal;
    lightCol = lightColor;
    lightPos = lightPosition;
    FragPos = vec3(model * vec4(position, 1.0));
    viewPos = cameraPosition;
    tmp = vec3(model[0][3], model[1][3], model[2][3]);
    ambientMat = ambiMat;
    diffuseMat = diffMat;
    specularMat = specMat;
}
