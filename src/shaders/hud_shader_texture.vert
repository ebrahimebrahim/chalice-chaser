#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 v_texcoord;

uniform mat4 model;

out vec2 f_texcoord;

void main()
{   
    f_texcoord = v_texcoord;
    gl_Position = model * vec4(position, 1.0);
}
