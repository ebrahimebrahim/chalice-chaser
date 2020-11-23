#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 player_pos;

out float distance_to_player;

void main()
{   
    vec4 world_pos = model * vec4(position,1.0);
    vec3 world_pos_dehomogenized = world_pos.xyz/world_pos.w;
    float dist_to_player = distance(world_pos_dehomogenized, player_pos);
    distance_to_player = dist_to_player;

    gl_Position = projection * view * world_pos;
}
