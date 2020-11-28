#version 330 core
layout (location = 0) out vec4 color;

in vec2 f_texcoord;

uniform sampler2D the_texture;

void main()
{
    color = texture(the_texture, f_texcoord);
}
