#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 uv;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform vec3 view_position;

out vec3 vert_color;
out vec3 normal;
out vec2 tex_coords;
out vec3 view_pos;
out vec3 frag_pos;

void main() {
    vec4 model_coords = model * vec4(aPos, 1.0);
    frag_pos = model_coords.xyz;
    gl_Position = projection * view * model_coords;
    vert_color = color;
    tex_coords = uv;
    view_pos = view_position;
    normal = mat3(transpose(inverse(model))) * norm;
}