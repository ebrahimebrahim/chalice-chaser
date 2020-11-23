#version 330 core
layout (location = 0) out vec4 color;

uniform vec3 player_pos;

in float distance_to_player;

void main()
{
    color = vec4(0.1, max(0.1, 0.5 - distance_to_player * 0.025), 0.25, 1.0);
}
