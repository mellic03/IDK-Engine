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
  bool is_active, is_shadowmapped;
  vec3 position;
  vec3 ambient, diffuse;
  float constant, linear, quadratic;
  float bias;
  float fog_constant, fog_linear, fog_quadratic;
  float fog_intensity;
  samplerCube depthCubemap;
  float radius;
};
uniform PointLight pointlights[NUM_POINTLIGHTS];



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

bool in_shadow(PointLight light, vec3 fragPos)
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


    // Point light
    //------------------------------------------------------------------
    for (int j=0; j<NUM_POINTLIGHTS; j++)
    {
      if (pointlights[j].is_active == false)
        continue;

      float d = length(ray - pointlights[j].position);
      // if (d > pointlights[j].radius)
      //   continue;
      float attenuation = 1.0 / (pointlights[j].fog_constant + d*pointlights[j].fog_linear + d*d*pointlights[j].fog_quadratic);
      attenuation *= 1.0 - (d/pointlights[j].radius);

      float v = attenuation * step_size * pointlights[j].fog_intensity;

      if (pointlights[j].is_shadowmapped)
      {
        if (!in_shadow(pointlights[j], ray))
        {
          pointlight_vol[j] += v;
        }
      }
      else
        pointlight_vol[j] += v;
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
    result += pointlight_vol[i] * pointlights[i].diffuse;
  
  result += dirlight_vol * shadowmapped_dirlight.diffuse;

  FragColor = vec4(result, 1.0);
}