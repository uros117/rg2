#version 330 core

uniform sampler2D ourTexture;

in vec3 vert_color;
in vec3 normal;
in vec2 tex_coords;
in vec3 view_pos;
in vec3 frag_pos;

out vec4 FragColor;

void main() {
    FragColor = texture(ourTexture, tex_coords) * vec4(vert_color, 1.0f);
    // FragColor = vec4(vert_color, 1.0f);
}