#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 7) in vec3 aAnimNextPos;

uniform float lerp_value;

uniform mat4 model;

void main()
{
  gl_Position = model * vec4(mix(aPos, aAnimNextPos, lerp_value), 1.0);
}