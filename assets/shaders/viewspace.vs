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

struct DirLight {
  vec3 direction;
  vec3 ambient, diffuse, specular;
};
uniform DirLight dirlights[NUM_DIRLIGHTS];

struct PointLight {
  vec3 position;
  vec3 ambient, diffuse, specular;
  float constant, linear, quadratic;
};
uniform PointLight pointlights[NUM_POINTLIGHTS];

struct SpotLight {
  vec3 position, direction;
  vec3 ambient, diffuse, specular;
  float constant, linear, quadratic;
  float inner_cutoff, outer_cutoff, intensity;
};
uniform SpotLight spotlights[NUM_POINTLIGHTS];


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightSpaceMatrix;

uniform vec3 viewPos;

out VS_OUT {
  vec3 FragPos; vec4 FragPosLightSpace;
  vec3 SurfaceNormal;
  vec2 TexCoords;
  
  vec3 DIR_TangentLightPositions[NUM_DIRLIGHTS];
  vec3 DIR_TangentViewPositions[NUM_DIRLIGHTS];
  vec3 DIR_TangentFragPositions[NUM_DIRLIGHTS];

  vec3 POINT_TangentLightPositions[NUM_POINTLIGHTS];
  vec3 POINT_TangentViewPositions[NUM_POINTLIGHTS];
  vec3 POINT_TangentFragPositions[NUM_POINTLIGHTS];

  vec3 SPOT_TangentLightPositions[NUM_SPOTLIGHTS];
  vec3 SPOT_TangentViewPositions[NUM_SPOTLIGHTS];
  vec3 SPOT_TangentFragPositions[NUM_SPOTLIGHTS];

} vs_out;

out mat3 TBNmat;

void main()
{
  vs_out.FragPos = vec3(inverse(view) * model * vec4(aPos, 1.0));
  vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
  vs_out.SurfaceNormal = aNormal;
  vs_out.TexCoords = aTexCoords;
  
  mat3 normalMatrix = transpose(inverse(mat3(inverse(view) * model)));
  vec3 T = normalize(normalMatrix * aTangent);
  vec3 N = normalize(normalMatrix * aNormal);
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);
  mat3 TBN = transpose(mat3(T, B, N));      
  TBNmat = TBN;
  for (int i=0; i<NUM_DIRLIGHTS; i++)
  {
    vs_out.DIR_TangentLightPositions[i] = TBN * dirlights[i].direction;
    vs_out.DIR_TangentViewPositions[i]  = TBN * viewPos;
    vs_out.DIR_TangentFragPositions[i]  = TBN * vs_out.FragPos;
  }

  for (int i=0; i<NUM_POINTLIGHTS; i++)
  {
    vs_out.POINT_TangentLightPositions[i] = TBN * pointlights[i].position;
    vs_out.POINT_TangentViewPositions[i]  = TBN * viewPos;
    vs_out.POINT_TangentFragPositions[i]  = TBN * vs_out.FragPos;
  }
  
  for (int i=0; i<NUM_SPOTLIGHTS; i++)
  {
    vs_out.SPOT_TangentLightPositions[i] = TBN * spotlights[i].position;
    vs_out.SPOT_TangentViewPositions[i]  = TBN * viewPos;
    vs_out.SPOT_TangentFragPositions[i]  = TBN * vs_out.FragPos;
  }
  

  gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}

