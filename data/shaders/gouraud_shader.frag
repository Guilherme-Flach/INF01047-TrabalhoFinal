#version 330 core

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D Texture0;

in vec3 illumination;

in vec2 texcoords;

out vec4 color;

void main()
{
    // Coordenadas de textura
    float U=texcoords.x;
    float V=texcoords.y;
    
    vec3 Kd0=texture(Texture0,vec2(U,V)).rgb;
    vec3 Ka=Kd0*.5;
    
    // Termo ambiente
    vec3 lambert_diffuse_term=Kd0*illumination;
    vec3 ambient_term=Ka*(.5);
    
    // Nao definimos termo especular devido as limitacoes de gouraud shading
    color.a=1.;
    
    color.rgb=lambert_diffuse_term+ambient_term;
    
    // Cor final com correção gamma, considerando monitor sRGB.
    color.rgb=pow(color.rgb,vec3(1.,1.,1.)/2.2);
}

