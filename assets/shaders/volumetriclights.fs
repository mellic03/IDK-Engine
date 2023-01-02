#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

struct Material {
  sampler2D diffuseMap, specularMap, emissionMap, normalMap;
  float spec_exponent;
};
uniform Material material;


void main()
{
  FragColor = texture(material.diffuseMap, TexCoords);
}