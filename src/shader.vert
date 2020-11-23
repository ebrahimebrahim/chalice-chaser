#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 player_pos;

void main()
{   
    vec4 world_pos = model * vec4(position,1.0);
    vec3 world_pos_dehomogenized = world_pos.xyz/world_pos.w;
    float dist_to_player = distance(world_pos_dehomogenized, player_pos);
    world_pos_dehomogenized += dist_to_player*vec3(0,1,0);

    gl_Position = projection * view * vec4(world_pos_dehomogenized,1);
}
