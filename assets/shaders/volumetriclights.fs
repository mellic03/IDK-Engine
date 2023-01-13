#version 330 core

#define NUM_POINTLIGHTS 10
#define NUM_SPOTLIGHTS 1

layout (location = 0) out vec4 FragColor;


struct VolumetricData {
  int num_samples;
  float step_size;
  float step_multiplier;
  
  int resolution_divisor;
  int num_blur_passes;
};
uniform VolumetricData volumetrics;


struct DirLight {
  vec3 position, direction;
  vec3 diffuse;
  vec3 ambient;
  float bias;
  float fog_intensity;
};
uniform DirLight shadowmapped_dirlight;


struct PointLight {
  vec3 position;
  vec3 ambient, diffuse;
  float constant, linear, quadratic;
  float bias;
  float fog_constant, fog_linear, fog_quadratic;
  float fog_intensity;
  float radius;
};
uniform PointLight volumetric_pointlights[NUM_POINTLIGHTS];
uniform int num_volumetric_pointlights;

struct ShadowPointLight {
  vec3 position;
  vec3 ambient, diffuse;
  float constant, linear, quadratic;
  float bias;
  float fog_constant, fog_linear, fog_quadratic;
  float fog_intensity;
  samplerCube depthCubemap;
  float radius;
};
uniform ShadowPointLight shadow_pointlights[NUM_POINTLIGHTS];
uniform int num_shadow_pointlights;


struct SpotLight {
  vec3 position, direction;
  vec3 ambient, diffuse;
  float constant, linear, quadratic;
  float inner_cutoff, outer_cutoff, intensity;
};
uniform SpotLight shadowmapped_spotlight;
uniform SpotLight spotlights[NUM_SPOTLIGHTS];
uniform int num_active_spotlights;

uniform mat4 dir_lightSpaceMatrix;

uniform sampler2D depthmap_dirlight;


vec3 gridSamplingDisk[20] = vec3[]
(
  vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
  vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
  vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
  vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
  vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

bool in_shadow(ShadowPointLight light, vec3 fragPos)
{
  vec3 fragToLight = fragPos - light.position;
  float closestDepth = texture(light.depthCubemap, fragToLight).r;
  closestDepth *= light.radius;
  float currentDepth = length(fragToLight);
  float bias = -0.0015;
  return currentDepth - bias > closestDepth;
}

float rand(vec2 co)
{
  return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

bool in_shadow_ortho(vec3 fragPos)
{
  vec4 lspacepos = dir_lightSpaceMatrix * vec4(fragPos, 1.0);

  vec3 projCoords = lspacepos.xyz / lspacepos.w;
  projCoords = projCoords * 0.5 + 0.5;
  
  float closestDepth = texture(depthmap_dirlight, projCoords.xy).r; 
  float currentDepth = projCoords.z;
  float bias = -0.0015;

  return currentDepth - bias > closestDepth;
}


in vec2 TexCoords;
uniform vec3 viewPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


void main()
{
  vec3 fragPos = texture(gPosition, TexCoords).rgb;

  float pointlight_vol[NUM_POINTLIGHTS];
  for (int i=0; i<NUM_POINTLIGHTS; i++)
    pointlight_vol[i] = 0.0;


  float dirlight_vol = 0.0;

  float frag_dist = length(fragPos - viewPos);

  vec3 ray;
  vec3 ray_dir = normalize(fragPos - viewPos);
  float ray_length = 0.0;

  float len = frag_dist / volumetrics.num_samples;
  len = clamp(len, 0.0, 0.2);

  for (int i=0; i<volumetrics.num_samples; i++)
  {
    float step_size = len;//i * (len/frag_dist);
    ray_length += step_size;

    if (ray_length > frag_dist)
      break;

    ray = viewPos + ray_length*ray_dir;


    // Point lights
    //------------------------------------------------------------------
    for (int j=0; j<NUM_POINTLIGHTS; j++)
    {
      if (j >= num_volumetric_pointlights)
        break;

      float d = length(ray - volumetric_pointlights[j].position);
      float attenuation = 1.0 / (volumetric_pointlights[j].fog_constant + d*volumetric_pointlights[j].fog_linear + d*d*volumetric_pointlights[j].fog_quadratic);
      
      attenuation *= 1.0 - clamp(d/volumetric_pointlights[j].radius, 0.0, 1.0);

      float vol = attenuation * step_size * volumetric_pointlights[j].fog_intensity;
      pointlight_vol[j] += vol;
    }

    for (int j=0; j<NUM_POINTLIGHTS; j++)
    {
      if (j >= num_shadow_pointlights)
        break;

      if (!in_shadow(shadow_pointlights[j], ray))
      {
        float d = length(ray - shadow_pointlights[j].position);
       
        float attenuation = 1.0 / (shadow_pointlights[j].fog_constant + d*shadow_pointlights[j].fog_linear + d*d*shadow_pointlights[j].fog_quadratic);
        attenuation *= 1.0 - (d/shadow_pointlights[j].radius);

        float vol = attenuation * step_size * shadow_pointlights[j].fog_intensity;
        pointlight_vol[num_volumetric_pointlights + j] += vol;
      }
    }
    //------------------------------------------------------------------

    // Directional light
    //------------------------------------------------------------------
    if (!in_shadow_ortho(ray))
      dirlight_vol += shadowmapped_dirlight.fog_intensity * step_size;
    //------------------------------------------------------------------
  }

  vec3 result = vec3(0.0, 0.0, 0.0);

  for (int i=0; i<NUM_POINTLIGHTS; i++)
  {
    if (i >= num_volumetric_pointlights)
      break;
    result += pointlight_vol[i] * volumetric_pointlights[i].diffuse;
  }
  
  for (int i=0; i<NUM_POINTLIGHTS; i++)
  {
    if (i >= num_shadow_pointlights)
      break;
    result += pointlight_vol[num_volumetric_pointlights + i] * shadow_pointlights[i].diffuse;
  }

  result += dirlight_vol * shadowmapped_dirlight.diffuse;

  FragColor = vec4(result, 1.0);
}