#version 430 core

in vec2 tex_coord;
in vec3 test;
layout(location = 0) out vec4 final_color;
uniform sampler2D sample_texture;

void main()
{
    //final_color = vec4(test.x, test.y, test.z, 1);
    final_color = vec4(1, 1, 0, 1);
    
    //final_color = texture(sample_texture, tex_coord);
}
