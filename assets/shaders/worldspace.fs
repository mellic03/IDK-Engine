#version 330 core

#define NUM_DIRLIGHTS 1
#define NUM_POINTLIGHTS 5
#define NUM_SPOTLIGHTS 2

struct Material {
  sampler2D diffuseMap, specularMap, emissionMap, normalMap;
  vec3 specularity;
  float spec_exponent;
};
uniform Material material;

struct DirLight {
  vec3 direction;
  vec3 ambient, diffuse, specular;
  float constant, linear, quadratic;
};
uniform DirLight dirlights[NUM_DIRLIGHTS];

struct PointLight {
  vec3 position;
  vec3 ambient, diffuse, specular;
  float constant, linear, quadratic;
};
uniform PointLight pointlights[NUM_POINTLIGHTS];

struct SpotLight {
  vec3 position, direction;
  vec3 ambient, diffuse, specular;
  float constant, linear, quadratic;
  float inner_cutoff, outer_cutoff;
};
uniform SpotLight spotlights[NUM_POINTLIGHTS];


in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
  
  vec3 DIR_TangentLightPositions[NUM_DIRLIGHTS];
  vec3 DIR_TangentViewPositions[NUM_DIRLIGHTS];
  vec3 DIR_TangentFragPositions[NUM_DIRLIGHTS];

  vec3 POINT_TangentLightPositions[NUM_POINTLIGHTS];
  vec3 POINT_TangentViewPositions[NUM_POINTLIGHTS];
  vec3 POINT_TangentFragPositions[NUM_POINTLIGHTS];

  vec3 SPOT_TangentLightPositions[NUM_SPOTLIGHTS];
  vec3 SPOT_TangentViewPositions[NUM_SPOTLIGHTS];
  vec3 SPOT_TangentFragPositions[NUM_SPOTLIGHTS];

} fs_in;

uniform vec3 viewDirection;
uniform vec3 viewPosition;
out vec4 FragColor;

vec3 calculate_dirlight(DirLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(-light.direction);

  // diffuse
  float diff = max(dot(normal, lightDir), 0.0);

  // specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.spec_exponent);

  // combine results
  vec3 ambient  = light.ambient  * vec3(texture(material.diffuseMap, fs_in.TexCoords));
  vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuseMap, fs_in.TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.specularMap, fs_in.TexCoords));
  return (ambient + diffuse + specular);
}

vec3 calculate_pointlight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - fragPos);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.spec_exponent);

  // attenuation
  float distance    = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  // combine results
  vec3 ambient  = light.ambient  * vec3(texture(material.diffuseMap, fs_in.TexCoords));
  vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuseMap, fs_in.TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.specularMap, fs_in.TexCoords));
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}

vec3 calculate_spotlight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - fragPos);
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.inner_cutoff - light.outer_cutoff;
  float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

  if(theta > light.outer_cutoff) 
  {
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.spec_exponent);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuseMap, fs_in.TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuseMap, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, fs_in.TexCoords));
    ambient  *= attenuation;
    diffuse  *= intensity * attenuation;
    specular *= intensity * attenuation;
    return (ambient + diffuse + specular);
  } 
  // else  // else, use ambient light so scene isn't completely dark outside the spotlight.
  //   return light.ambient * texture(material.diffuseMap, fs_in.TexCoords).rgb;
}

void main()
{

  vec3 norm = normalize(fs_in.Normal);
  vec3 viewDir = normalize(viewPosition - fs_in.FragPos);

  vec3 result = vec3(0.0);

  FragColor = texture(material.emissionMap, fs_in.TexCoords);

  for (int i=0; i<NUM_DIRLIGHTS; i++)
    result += calculate_dirlight(dirlights[i], fs_in.Normal, viewDir);

  for (int i=0; i<NUM_POINTLIGHTS; i++)
    result += calculate_pointlight(pointlights[i], fs_in.Normal, fs_in.FragPos, viewDir);

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
    result += calculate_spotlight(spotlights[i], fs_in.Normal, fs_in.FragPos, viewDir);

  FragColor += vec4(result, 1.0);

}

