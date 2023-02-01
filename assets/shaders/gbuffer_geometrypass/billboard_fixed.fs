#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 gEmission;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D diffuseMap;


void main()
{
  gAlbedoSpec = texture(diffuseMap, TexCoords);
  if (gAlbedoSpec.a < 0.1)
    discard;
  gAlbedoSpec.a = 0.0;

  gPosition = vec4(FragPos, 1.0);
  gNormal = vec4(Normal, 1.0);

  gEmission = vec3(0.0);
}
