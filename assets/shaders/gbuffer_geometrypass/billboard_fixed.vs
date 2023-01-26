#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec4 aColor;


out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
  vec4 worldPos = model * vec4(aPos, 1.0);
  FragPos = worldPos.xyz;
  TexCoords = aTexCoords;
  Normal = mat3(model) * aNormal;

  gl_Position = projection * view * worldPos;
}