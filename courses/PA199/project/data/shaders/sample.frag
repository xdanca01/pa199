#version 430 core

in vec2 tex_coord;
in vec3 normal_vec;
layout(location = 0) out vec4 final_color;
uniform sampler2D sample_texture;

void main()
{
    //Show normals
    final_color = vec4(normal_vec.x, normal_vec.y, normal_vec.z, 1);
    //Depth shadowing
    //final_color = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1);
    //final_color = vec4(tex_coord.x, 0, tex_coord.y, 1);
    //final_color = vec4(1, 1, 0, 1);
    
    //final_color = texture(sample_texture, tex_coord);
}
