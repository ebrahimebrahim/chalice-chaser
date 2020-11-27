#version 330 core
layout (location = 0) out vec4 color;

uniform vec3 player_pos;

in float distance_to_player;
in vec2 f_texcoord;

uniform sampler2D the_texture;

void main()
{
    vec3 color_rgb = (8.0 / (distance_to_player+4.0) ) * texture(the_texture, f_texcoord).rgb;
    color = vec4(color_rgb ,1.0);
}
