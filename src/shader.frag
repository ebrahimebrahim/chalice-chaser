#version 330 core
layout (location = 0) out vec4 color;

uniform vec3 player_pos;

in float distance_to_player;

void main()
{
    color = vec4(0.3, max(0.0, 1.0 - distance_to_player * 0.05), 0.5, 1.0);
}
