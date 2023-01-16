#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 gEmission;

in vec3 FragPos;
in vec2 TexCoords;
in vec4 Color;
in mat3 TBN;

struct TerrainMaterial {
  sampler2D   diffuseMap1,   diffuseMap2,   diffuseMap3,   diffuseMap4;
  sampler2D   specularMap1,  specularMap2,  specularMap3,  specularMap4;
  sampler2D   normalMap1,    normalMap2,    normalMap3,    normalMap4;
  float spec_exponent;
};
uniform TerrainMaterial material;

void main()
{
  gPosition = vec4(FragPos, 1.0);

  vec4 normal4 = Color.r * texture(material.normalMap1, TexCoords) +
                 Color.g * texture(material.normalMap2, TexCoords) +
                 Color.b * texture(material.normalMap3, TexCoords);
                //  Color.a * texture(material.normalMap4, TexCoords);
  vec3 normal = normal4.xyz;

  normal = normal * 2.0 - 1.0; 
  normal = normalize(TBN * normal);
  gNormal = vec4(normal, 1.0);


  gAlbedoSpec.rgb = Color.r * texture(material.diffuseMap1, TexCoords).rgb +
                    Color.g * texture(material.diffuseMap2, TexCoords).rgb +
                    Color.b * texture(material.diffuseMap3, TexCoords).rgb;
                    // Color.a * texture(material.diffuseMap4, TexCoords).rgb;

  gAlbedoSpec.a = 1.0;//texture(material.specularMap, TexCoords).r;

  gEmission = vec3(0.0);

}
