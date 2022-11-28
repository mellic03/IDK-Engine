#version 330 core

uniform float BIAS;

void main()
{             
  gl_FragDepth = gl_FragCoord.z;
  gl_FragDepth += gl_FrontFacing ? BIAS : 0.0; //  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
}
