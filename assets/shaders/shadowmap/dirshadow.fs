#version 330 core
#define BIAS 0

// layout (location = 0) out vec4 position;
// layout (location = 1) out vec4 normal;
// layout (location = 2) out vec4 flux;

// in vec4 pos;
// in vec4 norm;
in vec2 TexCoords;
uniform sampler2D diffuseMap;

void main()
{
  // if (texture(diffuseMap, TexCoords).a < 0.1)
  //   discard;

  gl_FragDepth = gl_FragCoord.z;
  gl_FragDepth += gl_FrontFacing ? BIAS : 0.0;

  // position.xyz  = pos.xyz;
  // normal.xyz    = normalize(norm.xyz);
  // flux .xyz     = texture(diffuseMap, TexCoords).rgb;
} 