#version 330 core
layout (location = 0) out vec4 color;

uniform vec3 player_pos;

in float distance_to_player;
in vec3 color_from_vert;

void main()
{
    vec3 color_rgb = (8.0 / (distance_to_player+4.0) ) * color_from_vert;
    color = vec4(color_rgb ,1.0);
}
