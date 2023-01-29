#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

layout (location = 4) in vec4 aColor;

layout (location = 5) in vec4 jointWeights;
layout (location = 6) in ivec4 jointIDs;


out vec2 TexCoords;
out vec3 FragPos;
out mat3 TBN;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
  vec4 worldPos = model * vec4(aPos, 1.0);
  FragPos = worldPos.xyz;
  TexCoords = aTexCoords;

  gl_Position = projection * view * worldPos;

  // tangent-space to world-space transform
  //------------------------------------------------------------------
  mat3 normalMatrix = mat3(model);
  vec3 T = normalize(normalMatrix * aTangent);
  vec3 N = normalize(normalMatrix * aNormal.xyz);
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);
  TBN = mat3(T, B, N);
  //------------------------------------------------------------------
}
