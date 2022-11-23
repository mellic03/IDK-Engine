#version 330 core

#define NUM_DIRLIGHTS 1
#define NUM_POINTLIGHTS 5
#define NUM_SPOTLIGHTS 2


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aFaceNormal;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;


out VS_OUT_POINTLIGHTS {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out_pointlights[NUM_POINTLIGHTS];

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct PointLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant, linear, quadratic;
};
uniform PointLight pointlights[NUM_POINTLIGHTS];

uniform vec3 viewPos;

void main()
{
  for (int i=0; i<8; i++)
  {
    vs_out_pointlights[i].FragPos = vec3(model * transform * vec4(aPos, 1.0));
    vs_out_pointlights[i].TexCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(model * transform)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    vs_out_pointlights[i].TangentLightPos = TBN * vec3(view * vec4(pointlights[i].position, 1.0));
    vs_out_pointlights[i].TangentViewPos  = TBN * viewPos;
    vs_out_pointlights[i].TangentFragPos  = TBN * vs_out_pointlights[i].FragPos;

  }
  gl_Position = projection * model * transform * vec4(aPos, 1.0);
}

