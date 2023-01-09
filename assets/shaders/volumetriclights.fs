#version 330 core

#define NUM_POINTLIGHTS 3
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
};
uniform PointLight shadowmapped_pointlight;
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

uniform samplerCube depthmap_pointlight;
uniform sampler2D depthmap_dirlight;
uniform float far_plane;


vec3 gridSamplingDisk[20] = vec3[]
(
  vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
  vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
  vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
  vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
  vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

bool in_shadow(vec3 lightPos, vec3 fragPos)
{
  vec3 fragToLight = fragPos - lightPos;
  float closestDepth = texture(depthmap_pointlight, fragToLight).r;
  closestDepth *= far_plane;
  float currentDepth = length(fragToLight);
  float bias = -0.0015;
  return currentDepth - bias > closestDepth;
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


  float pointlight_vol = 0.0;
  float dirlight_vol = 0.0;

  float frag_dist = length(fragPos - viewPos);

  float step_size = frag_dist / volumetrics.num_samples;
  step_size = clamp(step_size, 0.0, 0.2);
  float max_step_size = step_size;

  vec3 ray = viewPos;
  vec3 ray_dir = normalize(fragPos - viewPos);

  float ray_length = 0.0;

  for (int i=0; i<volumetrics.num_samples; i++)
  {
    ray += step_size * ray_dir;
    ray_length += step_size;

    if (ray_length > frag_dist)
      break;

    // Point light
    //------------------------------------------------------------------
    float light_dist = length(ray - shadowmapped_pointlight.position);
    float attenuation = 1.0 / (shadowmapped_pointlight.fog_constant + shadowmapped_pointlight.fog_linear * light_dist + shadowmapped_pointlight.fog_quadratic * (light_dist * light_dist));

    if (!in_shadow(shadowmapped_pointlight.position, ray))
      pointlight_vol += attenuation * step_size * shadowmapped_pointlight.fog_intensity;
    //------------------------------------------------------------------

    // Directional light
    //------------------------------------------------------------------
    if (!in_shadow_ortho(ray))
      dirlight_vol += shadowmapped_dirlight.fog_intensity * step_size;
    //------------------------------------------------------------------
  }

  vec3 result = vec3(0.0, 0.0, 0.0);

  result += pointlight_vol * shadowmapped_pointlight.diffuse;
  result += dirlight_vol * shadowmapped_dirlight.diffuse;

  FragColor = vec4(result, 1.0);
}