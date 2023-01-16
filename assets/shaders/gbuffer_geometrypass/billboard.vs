#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec4 aColor;
layout (location = 5) in mat4 aInstanceMatrix;

out vec2 TexCoords;
out vec3 FragPos;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 inv_view;


void main()
{
  vec4 worldPos = aInstanceMatrix * inv_view * vec4(aPos, 1.0);
  FragPos = worldPos.xyz;
  TexCoords = aTexCoords;

  gl_Position = projection * view * worldPos;

}
