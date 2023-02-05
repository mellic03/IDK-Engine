#version 330 core

#define BIAS 0

in vec2 TexCoords;
uniform sampler2D diffuseMap;

void main()
{
  if (texture(diffuseMap, TexCoords).a < 0.1)
    discard;

  gl_FragDepth = gl_FragCoord.z;
  gl_FragDepth += gl_FrontFacing ? BIAS : 0.0;
} 