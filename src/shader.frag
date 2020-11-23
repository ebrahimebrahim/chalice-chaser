#version 330 core
layout (location = 0) out vec4 color;

uniform vec3 player_pos;

in float distance_to_player;

void main()
{
    color = vec4(0.5, 0.4 + distance_to_player * 0.01, 0.8, 1.0);
}
