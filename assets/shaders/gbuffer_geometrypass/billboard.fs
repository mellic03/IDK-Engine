#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
// in vec3 FragPos;
// in vec3 Normal;

uniform sampler2D diffuseMap;


void main()
{
  FragColor = texture(diffuseMap, TexCoords);
  // if (FragColor.a < 0.1)
  //   discard;

  // gPosition = vec4(FragPos, 1.0);
  // gNormal.xyz = normalize(Normal);

  // gEmission = vec3(0.0);
}
