#version 330 core

#define NUM_DIRLIGHTS 1
#define NUM_POINTLIGHTS 5
#define NUM_SPOTLIGHTS 2


struct Material {
  sampler2D diffuseMap;
  sampler2D specularMap;
  sampler2D emissionMap;
  sampler2D normalMap;
  vec3 specularity;
  float spec_exponent;
};
uniform Material material;

struct DirLight {
  vec3 position;
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant, linear, quadratic;
};
uniform DirLight dirlights[NUM_DIRLIGHTS];

struct PointLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant, linear, quadratic;
};
uniform PointLight pointlights[NUM_POINTLIGHTS];

in VS_OUT_POINTLIGHTS {
  vec3 FragPos;
  vec2 TexCoords;
  vec3 TangentLightPos;
  vec3 TangentViewPos;
  vec3 TangentFragPos;
} fs_in[NUM_POINTLIGHTS];

out vec4 FragColor;

void main()
{
  vec3 aSpecular = texture(material.specularMap, fs_in[0].TexCoords).rgb;
  vec3 emission = texture(material.emissionMap, fs_in[0].TexCoords).rgb;
  vec3 color = texture(material.diffuseMap, fs_in[0].TexCoords).rgb;
  FragColor = vec4(emission, 1.0);

  for (int i=0; i<NUM_POINTLIGHTS; i++)
  {
    float distance = length(pointlights[i].position - fs_in[i].FragPos);
    float attenuation = 1.0 / (pointlights[i].constant + pointlights[i].linear*distance + pointlights[i].quadratic*distance*distance);

    vec3 normal = texture(material.normalMap, fs_in[i].TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    vec3 lightDir = normalize(fs_in[i].TangentLightPos - fs_in[i].TangentFragPos);
    vec3 viewDir = normalize(-fs_in[i].TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    
    float diff = max(dot(lightDir, normal), 0.0);

    vec3 diffuse  = diff * pointlights[i].diffuse * color;
    vec3 ambient  = pointlights[i].ambient * color;
    vec3 specular = pointlights[i].specular * spec;
    diffuse *= attenuation;
    ambient *= attenuation;
    specular *= attenuation;

    FragColor += vec4(ambient + diffuse + specular, 0.0);
  }

  float gamma = 1.2;
  FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));

}

