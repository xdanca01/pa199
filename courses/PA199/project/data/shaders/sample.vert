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

out vec2 tex_coord;
out vec3 normal_vec;
out vec3 lightCol;
out vec3 lightPos;
out vec3 FragPos;
out vec3 viewPos;

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
}
