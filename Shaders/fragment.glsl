#version 330 core
out vec4 FragColor;
in vec2 tex;
uniform sampler2D finaltex;

in vec4 color;
void main()
{
   FragColor = texture(finaltex, tex) * color;
}