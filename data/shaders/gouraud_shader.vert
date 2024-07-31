#version 330 core

layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D Texture0;

struct Material {
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float q;
};

uniform Material material;

out vec4 color_v;

void main()
{
    gl_Position = projection * view * model * model_coefficients;

    vec4 position_world = model * model_coefficients;
    
    vec4 normal = inverse(transpose(model)) * normal_coefficients;
    normal.w = 0.0;

    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.5,0.0));

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor que define o sentido da reflexão especular ideal.
    vec4 h = normalize(v+l);

    // Coordenadas de textura
    float U = texture_coefficients.x;
    float V = texture_coefficients.y;

    vec3 Kd0 = texture(Texture0, vec2(U,V)).rgb;

    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0,1.0,1.0); // PREENCH AQUI o espectro da fonte de luz

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2,0.2,0.2);

    // Termo difuso utilizando a lei dos cossenos de Lambert
    vec3 lambert_diffuse_term = Kd0 * material.Kd * I * max(0, dot(n,l)); 

    // Termo ambiente
    vec3 ambient_term = material.Ka * Ia;

    // Nao definimos termo especular devido as limitacoes de gouraud shading
    color_v.a = 1.0;

    color_v.rgb = lambert_diffuse_term + ambient_term;

    // Cor final com correção gamma, considerando monitor sRGB.
    color_v.rgb = pow(color_v.rgb, vec3(1.0,1.0,1.0)/2.2);

}

