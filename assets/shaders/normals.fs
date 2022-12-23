#version 330 core

uniform sampler2D diffuseMap;

in vec2 texCoord;
in vec3 normal;

out vec4 fragColor;

void main()
{
  vec3 color = texture(diffuseMap, texCoord).rgb;

  fragColor = vec4(color, 1.0);
}