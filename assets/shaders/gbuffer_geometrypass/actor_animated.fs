#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gEmission;


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;


struct Material {
  sampler2D diffuseMap, specularMap, emissionMap, normalMap;
  float spec_exponent;
};
uniform Material material;

uniform vec3 emission;
uniform float emission_scale;


void main()
{
  gPosition = vec4(FragPos, 1.0);

  gNormal = vec4(Normal, 1.0);


  gAlbedoSpec.rgb = texture(material.diffuseMap, TexCoords).rgb ;
  gAlbedoSpec.a = 50.0 * texture(material.specularMap, TexCoords).r;

  gEmission = emission_scale * (vec4(emission, 1.0) * (texture(material.emissionMap, TexCoords) + vec4(gAlbedoSpec.rgb, 1.0)));
}
