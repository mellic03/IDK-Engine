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
};
uniform DirLight shadowmapped_dirlight;


struct PointLight {
  vec3 position;
  vec3 ambient, diffuse;
  float constant, linear, quadratic;
  float bias;
  float fog_constant, fog_linear, fog_quadratic;
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


in VS_OUT {
  vec4 lspacepos;
  vec3 FragPos;
  vec3 viewPos;
} fs_in;


struct Material {
  sampler2D diffuseMap, specularMap, emissionMap, normalMap;
  float spec_exponent;
};
uniform Material material;

uniform mat4 lightSpaceMatrix;
uniform samplerCube depthmap_pointlight;
uniform sampler2D   depthmap_dirlight;
uniform sampler2D   depthmap_spotlight;
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
  vec4 lspacepos = lightSpaceMatrix * vec4(fragPos, 1.0);
  vec3 projCoords = lspacepos.xyz / lspacepos.w;
  projCoords = projCoords * 0.5 + 0.5;
  float closestDepth = texture(depthmap_dirlight, projCoords.xy).r; 
  float currentDepth = projCoords.z;
  float bias = -0.0015; 
  return currentDepth - bias > closestDepth;
}


vec3 calculate_dirlight(DirLight light, vec3 fragPos, vec3 viewPos)
{
  // for each step, move the endpoint of the ray towards the fragment
  vec3 vol = vec3(0, 0, 0);

  // float frag_dist = length(fs_in.FragPos - fs_in.viewPos);
  // int max_steps = 512;
  // float step_size = 0.1;

  // vec3 ray = fs_in.viewPos;
  // vec3 ray_dir = normalize(fs_in.FragPos - fs_in.viewPos);

  // int i=0;
  // for (i=0; i<max_steps; i++)
  // {
  //   ray += step_size * ray_dir;

  //   if (length(ray - fs_in.viewPos) > frag_dist)
  //     break;

  //   if (!in_shadow_ortho(viewPos, ray))
  //     vol += step_size * 0.25 * light.diffuse;
  // }

  return vol;
}

vec3 calculate_pointlight(PointLight light, vec3 fragPos, vec3 viewPos)
{
  // for each step, move the endpoint of the ray towards the fragment
  vec3 vol = vec3(0, 0, 0);

  // float frag_dist = length(fs_in.FragPos - fs_in.viewPos);
  // int max_steps = 512;
  // float step_size = 0.1;

  // vec3 ray = fs_in.viewPos;
  // vec3 ray_dir = normalize(fs_in.FragPos - fs_in.viewPos);

  // int i=0;
  // for (i=0; i<max_steps; i++)
  // {
  //   ray += step_size * ray_dir;

  //   if (length(ray - fs_in.viewPos) > frag_dist)
  //     break;

  //   float light_dist = length(ray - light.position);
  //   float attenuation = 1.0 / (light.fog_constant + light.fog_linear * light_dist + light.fog_quadratic * (light_dist * light_dist));

  //   if (!in_shadow(light.position, viewPos, ray))
  //     vol += step_size * 0.25 * attenuation * light.diffuse;
  // }

  return vol;
}

float rand(vec2 co)
{
  return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}


vec3 calculate_spotlight(SpotLight light, vec3 fragPos, vec3 viewPos)
{
  vec3 vol = vec3(0.0);

  // float frag_dist = length(fs_in.FragPos - fs_in.viewPos);
  // int max_steps = 128;
  // float min_step_size = 0.01;
  // float step_size = frag_dist / max_steps;
  // step_size = step_size > min_step_size ? step_size : min_step_size;
  // int steps = 0;

  // vec3 ray = fs_in.viewPos;
  // vec3 ray_dir = normalize(fs_in.FragPos - fs_in.viewPos);

  // for (int i=0; i<max_steps; i++)
  // {
  //   ray += step_size * ray_dir;

  //   if (length(ray - fs_in.viewPos) > frag_dist)
  //     break;


  //   vec3 lightDir = normalize(light.position - ray);
  //   float theta = dot(lightDir, normalize(-light.direction));
  //   float epsilon = light.inner_cutoff - light.outer_cutoff;
  //   float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0); 
  //   intensity *= light.intensity;

  //   float distance    = length(light.position - fragPos);
  //   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  //   attenuation *= intensity;

  //   if (!in_shadow(light.position, viewPos, ray))
  //     vol += 0.008 * attenuation * light.diffuse;
  // }

  return vol;
}


void main()
{
  float pointlight_vol = 0.0;
  float dirlight_vol = 0.0;

  float frag_dist = length(fs_in.FragPos - fs_in.viewPos);

  float step_size = frag_dist / volumetrics.num_samples;
  step_size = clamp(step_size, 0.0, 0.2);
  // float step_size = frag_dist / volumetrics.num_samples;
  float max_step_size = step_size;

  vec3 ray = fs_in.viewPos;
  vec3 ray_dir = normalize(fs_in.FragPos - fs_in.viewPos);

  float ray_length = 0.0;

  for (int i=0; i<volumetrics.num_samples; i++)
  {
    ray += step_size * ray_dir;
    ray_length += step_size;

    // step_size = max_step_size * (exp(ray_length / frag_dist));

    if (ray_length > frag_dist)
      break;

    // Point light
    //------------------------------------------------------------------
    float light_dist = length(ray - shadowmapped_pointlight.position);
    float attenuation = 1.0 / (shadowmapped_pointlight.fog_constant + shadowmapped_pointlight.fog_linear * light_dist + shadowmapped_pointlight.fog_quadratic * (light_dist * light_dist));

    if (!in_shadow(shadowmapped_pointlight.position, ray))
      pointlight_vol += attenuation * step_size;
    //------------------------------------------------------------------


    // Directional light
    //------------------------------------------------------------------
    if (!in_shadow_ortho(ray))
      dirlight_vol += 0.05 * step_size;
    //------------------------------------------------------------------

  }

  vec3 result = vec3(0.0, 0.0, 0.0);

  result += pointlight_vol * shadowmapped_pointlight.diffuse;
  result += dirlight_vol * shadowmapped_dirlight.diffuse;
  
  FragColor = vec4(result, 1.0);
}