#version 330 core

#define NUM_POINTLIGHTS 10
#define NUM_SPOTLIGHTS 1

struct Material {
  sampler2D diffuseMap, specularMap, emissionMap, normalMap;
  float spec_exponent;
};
uniform Material material;


struct DirLight {
  vec3 position, direction;
  vec3 diffuse;
  vec3 ambient;
  float bias;
  float fog_intensity;
};
uniform DirLight shadowmapped_dirlight;


struct PointLight {
  float radius;
  vec3 ambient, diffuse;
  vec3 position;
  float constant, linear, quadratic;
  float bias;
  float fog_constant, fog_linear, fog_quadratic;
  float fog_intensity;
};
uniform PointLight pointlights[NUM_POINTLIGHTS];
uniform int num_active_pointlights;

struct ShadowPointLight {
  float radius;
  vec3 ambient, diffuse;
  vec3 position;
  float constant, linear, quadratic;
  float bias;
  float fog_constant, fog_linear, fog_quadratic;
  float fog_intensity;
  samplerCube depthCubemap;
};
uniform ShadowPointLight shadow_pointlights[NUM_POINTLIGHTS];
uniform int num_shadow_pointlights;


layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gEmission;

uniform mat4 dir_lightSpaceMatrix;
uniform sampler2D depthmap_dirlight;

uniform vec3 viewPos;


vec3 gridSamplingDisk[20] = vec3[]
(
  vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
  vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
  vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
  vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
  vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);
float calculate_shadow_pointlight(ShadowPointLight light, vec3 viewPos, vec3 fragPos)
{
  vec3 fragToLight = fragPos - light.position;
  float currentDepth = length(fragToLight);

  float shadow = 0.0;
  int samples = 20;
  float viewDistance = length(viewPos - fragPos);
  float diskRadius = (1.0 + (viewDistance / light.radius)) / 25.0;
   
  for(int i = 0; i < samples; ++i)
  {
    float closestDepth = texture(light.depthCubemap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
    closestDepth *= light.radius;   // undo mapping [0;1]
    if(currentDepth - light.bias > closestDepth)
      shadow += 1.0;
  }


  shadow /= float(samples);

  return shadow;
}


float calculate_shadow_dirlight(vec3 lightPos, vec3 fragPos, vec3 normal, vec3 lightDir, float bias)
{
  vec4 lspacepos = dir_lightSpaceMatrix * vec4(fragPos, 1.0);

  vec3 projCoords = lspacepos.xyz / lspacepos.w;
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.z > 1.0)
    return 0.0;

  float closestDepth = texture(depthmap_dirlight, projCoords.xy).r; 
  float currentDepth = projCoords.z;
  
  float diffuseFactor = dot(normal, -lightDir);
  float bb = 0.01;

  float shadow = 0.0;
  vec2 texelSize = 2.0 / textureSize(depthmap_dirlight, 1);
  const int halfkernelWidth = 2;
  for(int x = -halfkernelWidth; x <= halfkernelWidth; ++x)
  {
    for(int y = -halfkernelWidth; y <= halfkernelWidth; ++y)
    {
      float pcfDepth = texture(depthmap_dirlight, projCoords.xy + vec2(x, y) * texelSize).r;
      shadow += currentDepth - bb > pcfDepth ? 1.0 : 0.0;
    }
  }
  shadow /= ((halfkernelWidth*2+1)*(halfkernelWidth*2+1));

  return shadow;
}


vec3 calculate_shadowmapped_pointlight(ShadowPointLight light, vec3 albedo, vec3 fragPos, vec3 normal, float spec_strength)
{
  float d = length(light.position - fragPos);
  if (d > light.radius)
    return vec3(0.0);

  vec3 lightDir = normalize(light.position - fragPos);
  vec3 viewDir = normalize(viewPos - fragPos);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 halfwayDir = normalize(lightDir + viewDir);  
  float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
  
  float attenuation = 1.0 / (light.constant + d*light.linear + d*d*light.quadratic);
  attenuation *= 1.0 - clamp(d/light.radius, 0.0, 1.0);

  vec3 diffuse  = attenuation * albedo * diff * light.diffuse;
  vec3 ambient  = attenuation * albedo * light.ambient;
  vec3 specular = attenuation * albedo * spec * light.diffuse * spec_strength;

  float shadow = calculate_shadow_pointlight(light, viewPos, fragPos);
  return  (ambient + (1.0 - shadow) * (diffuse + specular));
}


vec3 calculate_pointlight(PointLight light, vec3 albedo, vec3 fragPos, vec3 normal, float spec_strength)
{
  float d = length(light.position - fragPos);
  if (d > light.radius)
    return vec3(0.0);

  vec3 lightDir = normalize(light.position - fragPos);
  vec3 viewDir = normalize(viewPos - fragPos);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 halfwayDir = normalize(lightDir + viewDir);  
  float spec = pow(max(dot(normal, halfwayDir), 0.0), 16);
  
  float attenuation = 1.0 / (light.constant + d*light.linear + d*d*light.quadratic);
  attenuation *= 1.0 - d/light.radius;

  vec3 diffuse  = attenuation * albedo * diff * light.diffuse;
  vec3 ambient  = attenuation * albedo * light.ambient;
  vec3 specular = attenuation * albedo * spec * light.diffuse * spec_strength;

  return  (ambient + diffuse + specular);
}


vec3 calculate_dirlight(DirLight light, vec3 albedo, vec3 fragPos, vec3 normal, float spec_strength)
{
  vec3 lightDir = normalize(light.position);  
  float diff = max(dot(normal, lightDir), 0.0);

  vec3 halfwayDir = normalize(lightDir + normalize(viewPos - fragPos));  
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.spec_exponent);
  
  vec3 ambient  = albedo * light.ambient;
  vec3 diffuse  = albedo * diff * light.diffuse;
  vec3 specular = albedo * spec * light.diffuse * 0;

  float shadow = calculate_shadow_dirlight(light.position, fragPos, normal, normalize(-light.position), light.bias);

  return (ambient + (1.0 - shadow) * (diffuse + specular));
}


void main()
{
  vec3 fragPos = texture(gPosition, TexCoords).rgb;
  vec3 normal = texture(gNormal, TexCoords).rgb;
  vec3 albedo = texture(gAlbedoSpec, TexCoords).rgb;
  vec3 emission = texture(gEmission, TexCoords).rgb;
  float specular_map = texture(gAlbedoSpec, TexCoords).a;

  vec3 result = vec3(0.0);
  result += emission;

  for (int i=0; i<NUM_POINTLIGHTS; i++)
  {
    if (i >= num_shadow_pointlights)
      break;

    result += calculate_shadowmapped_pointlight(shadow_pointlights[i], albedo, fragPos, normal, specular_map);
  }

  for (int i=0; i<num_active_pointlights; i++)
  {
    if (i >= num_active_pointlights)
      break;

    result += calculate_pointlight(pointlights[i], albedo, fragPos, normal, specular_map);
  }


  result += calculate_dirlight(shadowmapped_dirlight, albedo, fragPos, normal, specular_map);

  FragColor = vec4(result, 1.0);

  float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
  if(brightness > 1.0)
    BrightColor = vec4(FragColor.rgb, 1.0);
  else
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

