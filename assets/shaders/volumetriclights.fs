#version 330 core

#define NUM_POINTLIGHTS 3
#define NUM_SPOTLIGHTS 1

layout (location = 0) out vec4 FragColor;


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

bool in_shadow(vec3 lightPos, vec3 viewPos, vec3 fragPos)
{
  vec3 fragToLight = fragPos - lightPos;
  float closestDepth = texture(depthmap_pointlight, fragToLight).r;
  closestDepth *= far_plane;
  float currentDepth = length(fragToLight);
  float bias = 0.05; 
  return currentDepth - bias > closestDepth;
}

bool in_shadow_ortho(vec3 viewPos, vec3 fragPos)
{
  vec4 lspacepos = lightSpaceMatrix * vec4(fragPos, 1.0);
  vec3 projCoords = lspacepos.xyz / lspacepos.w;
  projCoords = projCoords * 0.5 + 0.5;
  float closestDepth = texture(depthmap_dirlight, projCoords.xy).r; 
  float currentDepth = projCoords.z;
  return currentDepth > closestDepth;
}


vec3 calculate_dirlight(DirLight light, vec3 fragPos, vec3 viewPos)
{
  // for each step, move the endpoint of the ray towards the fragment
  vec3 vol = vec3(0, 0, 0);

  float view_frag_length = length(fs_in.FragPos - fs_in.viewPos);
  int max_steps = 128;
  float step_size = view_frag_length / max_steps;

  vec3 ray = fs_in.viewPos;
  vec3 ray_dir = normalize(fs_in.FragPos - fs_in.viewPos);

  int i=0;
  for (i=0; i<max_steps; i++)
  {
    ray += step_size * ray_dir;

    if (length(ray - fs_in.viewPos) > view_frag_length)
      break;

    if (!in_shadow_ortho(viewPos, ray))
      vol += step_size * 0.25 * light.diffuse;
  }

  return (vol);
}

vec3 calculate_pointlight(PointLight light, vec3 fragPos, vec3 viewPos)
{
  // for each step, move the endpoint of the ray towards the fragment
  vec3 vol = vec3(0, 0, 0);

  float view_frag_length = length(fs_in.FragPos - fs_in.viewPos);
  int max_steps = 512;
  float step_size = 0.1;

  vec3 ray = fs_in.viewPos;
  vec3 ray_dir = normalize(fs_in.FragPos - fs_in.viewPos);

  int i=0;
  for (i=0; i<max_steps; i++)
  {
    ray += step_size * ray_dir;

    if (length(ray - fs_in.viewPos) > view_frag_length)
      break;

    float light_ray_dist = length(ray - light.position);
    float attenuation = 1.0 / (light.fog_constant + light.fog_linear * light_ray_dist + light.fog_quadratic * (light_ray_dist * light_ray_dist));

    if (!in_shadow(light.position, viewPos, ray))
      vol += step_size * 0.25 * attenuation * light.diffuse;
  }

  return (vol);
}


vec3 calculate_spotlight(SpotLight light, vec3 fragPos, vec3 viewPos)
{
  vec3 vol;

  float view_frag_length = length(fs_in.FragPos - fs_in.viewPos);
  int max_steps = 128;
  float min_step_size = 0.01;
  float step_size = view_frag_length / max_steps;
  step_size = step_size > min_step_size ? step_size : min_step_size;
  int steps = 0;

  vec3 ray = fs_in.viewPos;
  vec3 ray_dir = normalize(fs_in.FragPos - fs_in.viewPos);

  for (int i=0; i<max_steps; i++)
  {
    ray += step_size * ray_dir;

    if (length(ray - fs_in.viewPos) > view_frag_length)
      break;


    vec3 lightDir = normalize(light.position - ray);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.inner_cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0); 
    intensity *= light.intensity;

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    attenuation *= intensity;

    if (!in_shadow(light.position, viewPos, ray))
      vol += 0.008 * attenuation * light.diffuse;
  }

  return vol;
}


void main()
{
  vec3 result = vec3(0.0, 0.0, 0.0);

  // result += calculate_pointlight(shadowmapped_pointlight, fs_in.FragPos, fs_in.viewPos);
  result += calculate_dirlight(shadowmapped_dirlight, fs_in.FragPos, fs_in.viewPos);

  // for (int i=0; i<num_active_spotlights; i++)
  //   result += calculate_spotlight(spotlights[i], fs_in.FragPos, fs_in.viewPos);

  FragColor = vec4(result, 1.0);

}