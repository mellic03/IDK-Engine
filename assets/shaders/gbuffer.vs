#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;


out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out mat3 TBN;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
  FragPos = vec3(model * vec4(aPos, 1.0));
  Normal =  aNormal;
  TexCoords = aTexCoords;

  gl_Position = projection * view * model * vec4(aPos, 1.0);

  // tangent-space to world-space transform
  //------------------------------------------------------------------
  mat3 normalMatrix = mat3(model);
  vec3 T = normalize(normalMatrix * aTangent);
  vec3 N = normalize(normalMatrix * aNormal);
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);
  TBN = mat3(T, B, N);
  //------------------------------------------------------------------
}
