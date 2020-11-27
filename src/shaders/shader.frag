#version 330 core
layout (location = 0) out vec4 color;

uniform vec3 player_pos;
uniform vec3 object_color;

in float distance_to_player;

void main()
{
    vec3 color_rgb = (8.0 / (distance_to_player+4.0) ) * object_color;
    color = vec4(color_rgb ,1.0);
}
