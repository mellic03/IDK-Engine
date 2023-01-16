#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec4 aColor;
layout (location = 5) in mat4 aInstanceMatrix;

out vec2 TexCoords;
// out vec3 FragPos;
// out vec3 Normal;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 view_noTranslate;


void main()
{
  vec4 worldPos = aInstanceMatrix * inverse(view_noTranslate) * vec4(aPos, 1.0);
  // FragPos = worldPos.xyz;
  TexCoords = aTexCoords;
  // Normal = (aInstanceMatrix * inverse(view_noTranslate) * vec4(aNormal.xyz, 0.0)).xyz;

  gl_Position = projection * view * worldPos;
}