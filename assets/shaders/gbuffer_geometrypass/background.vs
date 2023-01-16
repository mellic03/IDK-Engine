#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
  vec4 worldPos = inverse(view) * model * vec4(aPos, 1.0);
  FragPos = worldPos.xyz;
  TexCoords = TexCoords;

  gl_Position = projection * view * worldPos;
}
