#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;


out VS_OUT {

  vec4 lspacepos;

  vec3 FragPos;
  vec3 viewPos;
  vec3 Normal;
  vec2 TexCoords;

  mat3 TBN;

} vs_out;

uniform vec3 viewPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 lightSpaceMatrix;

void main()
{

  vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
  vs_out.viewPos = viewPos;
  vs_out.Normal =  aNormal;
  vs_out.TexCoords = aTexCoords;
  gl_Position = projection * view * model * vec4(aPos, 1.0);


  vs_out.lspacepos = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);


  // tangent-space to world-space transform
  //------------------------------------------------------------------
  mat3 normalMatrix = mat3(model);
  vec3 T = normalize(normalMatrix * aTangent);
  vec3 N = normalize(normalMatrix * aNormal);
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);
  vs_out.TBN = (mat3(T, B, N));
  //------------------------------------------------------------------

}