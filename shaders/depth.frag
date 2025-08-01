#version 330 core

uniform sampler2D shadowMap;

in vec3 vert_color;
in vec3 normal;
in vec2 tex_coords;
in vec3 view_pos;
in vec3 frag_pos;

out vec4 FragColor;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float near_plane = 0.1;
    float far_plane = 30.0;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{             
    float depthValue = texture(shadowMap, tex_coords).r;
    // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}