#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform vec3 diffuseColor;

out vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
  color = diffuseColor;
  
  gl_Position = projection * view * model * vec4(aPos, 1.0);
}