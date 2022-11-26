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
  float inner_cutoff, outer_cutoff, intensity;
};
uniform SpotLight spotlights[NUM_POINTLIGHTS];


in VS_OUT {
  vec3 FragPos; vec4 FragPosLightSpace;
  vec3 SurfaceNormal;
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

in mat3 TBNmat;

uniform float BIAS;
uniform vec3 viewPos;
uniform sampler2D shadowMap;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

float calculate_shadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.z > 1.0)
    return 0.0;

  float closestDepth = texture(shadowMap, projCoords.xy).r; 
  float currentDepth = projCoords.z;
  
  float bias = max(BIAS * (1.0 - dot(normal, lightDir)), 0.0005);

  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
  for(int x = -1; x <= 1; ++x)
  {
      for(int y = -1; y <= 1; ++y)
      {
          float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
          shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
      }    
  }
  shadow /= 9.0;

  return shadow;
}

vec3 calculate_dirlight(DirLight light, vec3 normal, vec3 viewDir, int index)
{
  vec3 ambient = light.ambient * texture(material.diffuseMap, fs_in.TexCoords).rgb;
  
  // diffuse 
  vec3 lightDir = normalize(-light.direction);  
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(material.diffuseMap, fs_in.TexCoords).rgb;  
  
  // specular
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.spec_exponent);
  vec3 specular = light.specular * spec * texture(material.specularMap, fs_in.TexCoords).rgb;
  
  float shadow = calculate_shadow(fs_in.FragPosLightSpace, normal, lightDir);
  return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 calculate_pointlight(PointLight light, vec3 normal, vec3 fragPos, int index)
{
  vec3 lightDir = normalize(fs_in.POINT_TangentLightPositions[index] - fs_in.POINT_TangentFragPositions[index]);
  vec3 viewDir = normalize(fs_in.POINT_TangentViewPositions[index] - fs_in.POINT_TangentFragPositions[index]);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // specular shading
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.spec_exponent);

  // attenuation
  float distance    = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  // combine results
  vec3 ambient  = light.ambient * texture(material.diffuseMap, fs_in.TexCoords).rgb;
  vec3 diffuse  = light.diffuse * diff * texture(material.diffuseMap, fs_in.TexCoords).rgb;
  vec3 specular = light.diffuse * spec * texture(material.specularMap, fs_in.TexCoords).rgb;

  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}

vec3 calculate_spotlight(SpotLight light, vec3 normal, vec3 fragPos, int index)
{
  vec3 lightDir = normalize(light.position - fragPos);
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.inner_cutoff - light.outer_cutoff;
  float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0); 
  intensity *= light.intensity;

    lightDir = normalize(fs_in.SPOT_TangentLightPositions[index] - fs_in.SPOT_TangentFragPositions[index]);
    vec3 viewDir = normalize(fs_in.SPOT_TangentViewPositions[index] - fs_in.SPOT_TangentFragPositions[index]);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.spec_exponent);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuseMap, fs_in.TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuseMap, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, fs_in.TexCoords));
    ambient  *= intensity * attenuation;
    diffuse  *= intensity * attenuation;
    specular *= intensity * attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
  vec3 fragNormal = texture(material.normalMap, fs_in.TexCoords).rgb;
  fragNormal = normalize(2.0 * fragNormal - 1.0);

  vec3 viewDir = fs_in.FragPos - viewPos;

  vec3 result = vec3(0.0);

  FragColor = texture(material.emissionMap, fs_in.TexCoords);


  for (int i=0; i<NUM_DIRLIGHTS; i++)
    result += calculate_dirlight(dirlights[i], inverse(TBNmat) * fragNormal, viewDir, i);

  for (int i=0; i<NUM_POINTLIGHTS; i++)
    result += calculate_pointlight(pointlights[i], fragNormal, fs_in.FragPos, i);

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
    result += calculate_spotlight(spotlights[i], fragNormal, fs_in.FragPos, i);

  FragColor += vec4(result, 0.0);

  float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
  if(brightness > 1.0)
      BrightColor = vec4(FragColor.rgb, 1.0);
  else
      BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

