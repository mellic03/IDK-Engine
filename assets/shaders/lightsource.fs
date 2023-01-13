#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 gEmission;


in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;


struct Material {
  sampler2D diffuseMap, specularMap, emissionMap, normalMap;
  float spec_exponent;
};
uniform Material material;

uniform vec3 diffuseColor;

void main()
{
  gPosition = vec4(FragPos, 0.0);

  gNormal = vec4(0.0, 0.0, 0.0, 0.0);

  gAlbedoSpec.rgb = diffuseColor;
  gAlbedoSpec.a = 1.0;

  gEmission = diffuseColor;
}
