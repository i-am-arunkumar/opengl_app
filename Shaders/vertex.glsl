#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 acolor;
layout (location = 2) in vec2 atex;
out vec4 color;
out vec2 tex;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 offset;

void main()
{
    gl_Position = projection * view * model * offset * vec4(aPos, 1.0);
	color = vec4(acolor,1.0);
	tex = atex;
}