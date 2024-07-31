#version 330 core

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec4 color_v;

out vec4 color;

void main()
{
    color = color_v; 
} 

