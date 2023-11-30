#version 430 core

in vec2 tex_coord;
in vec3 normal_vec;
in vec3 lightCol;
in vec3 lightPos;
in vec3 FragPos;
in vec3 viewPos;
in vec3 tmp;
in vec3 ambientMat;
in vec3 diffuseMat;
in vec3 specularMat;


layout(location = 0) out vec4 final_color;
uniform sampler2D sample_texture;
uniform bool useTexture;

void main()
{
    vec3 normal = normalize(normal_vec);

    //Show normals
    //vec3 color = vec3(normal.x, normal.y, normal.z);
    //vec3 color = vec3(FragPos.x, FragPos.y, FragPos.z);
    //Need to setup color, because of textures
    vec3 color = vec3(1.0, 1.0, 1.0);
    if(useTexture)
    {
        color = texture(sample_texture, tex_coord).xyz;
    }

    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightCol * diffuseMat;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightCol * ambientMat;

    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightCol * specularMat;

    final_color = vec4((ambient + diffuse + specular) * color, 1.0);
    //Gamma correction
    /*float gamma = 2.2;
    final_color.rgb = pow(final_color.rgb, vec3(1.0/gamma));*/
    
    
    //Depth shadowing
    //final_color = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1);
    //final_color = vec4(tex_coord.x, 0, tex_coord.y, 1);
    //final_color = vec4(1, 1, 0, 1);
    
    //final_color = texture(sample_texture, tex_coord);
}
