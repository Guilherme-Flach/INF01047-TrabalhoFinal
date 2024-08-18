#version 330 core

layout(location=0)in vec4 model_coefficients;
layout(location=1)in vec4 normal_coefficients;
layout(location=2)in vec2 texture_coefficients;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 illumination;

out vec2 texcoords;

void main()
{
    gl_Position=projection*view*model*model_coefficients;
    
    vec4 position_world=model*model_coefficients;
    
    vec4 normal=inverse(transpose(model))*normal_coefficients;
    normal.w=0.;
    
    vec4 origin=vec4(0.,0.,0.,1.);
    vec4 camera_position=inverse(view)*origin;
    
    vec4 p=position_world;
    
    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n=normalize(normal);
    
    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.03, -7.03, 15.12, 1.0));
    
    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v=normalize(camera_position-p);
    
    // Vetor que define o sentido da reflexão especular ideal.
    vec4 h=normalize(v+l);
    
    // Espectro da fonte de iluminação
    vec3 I=vec3(1.,1.,1.);// PREENCH AQUI o espectro da fonte de luz
    
    // Espectro da luz ambiente
    vec3 Ia=vec3(.2,.2,.2);
    
    // Termo difuso utilizando a lei dos cossenos de Lambert
    illumination=I*max(0,dot(n,l));
    
    texcoords=texture_coefficients;
}

