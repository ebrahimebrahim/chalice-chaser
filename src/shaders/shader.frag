#version 330 core
layout (location = 0) out vec4 color;

uniform vec3 player_pos;

in float distance_to_player;

void main()
{
    vec3 color_rgb = (8.0 / (distance_to_player+4.0) ) * vec3(0.4, 0.5, 0.4);
    color = vec4(color_rgb ,1.0);
}
