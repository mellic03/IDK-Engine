#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gTangent;
layout (location = 3) out vec4 gAlbedoSpec;


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;


struct Material {
  sampler2D diffuseMap, specularMap, emissionMap, normalMap;
  float spec_exponent;
};
uniform Material material;


void main()
{
  gPosition = FragPos;
  gNormal = normalize(Normal);
  gTangent = TBN * gNormal;
  gAlbedoSpec.rgb = texture(material.diffuseMap, TexCoords).rgb;
  gAlbedoSpec.a = texture(material.specularMap, TexCoords).r;
}
