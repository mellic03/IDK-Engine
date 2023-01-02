#version 330 core

#define NUM_SPOTLIGHTS 2
layout (location = 0) out vec4 FragColor;

struct PointLight {
  vec3 pos;
  vec3 ambient, diffuse;
  float constant, linear, quadratic;
  float bias;
};
uniform PointLight pointlight;

struct SpotLight {
  vec3 position, direction;
  vec3 ambient, diffuse;
  float constant, linear, quadratic;
  float inner_cutoff, outer_cutoff, intensity;
};
uniform SpotLight spotlights[NUM_SPOTLIGHTS];
uniform int num_active_spotlights;

in VS_OUT {

  vec3 FragPos;
  vec3 viewPos;
  vec3 Normal;
  vec2 TexCoords;

  mat3 TBN;

} fs_in;


struct Material {
  sampler2D diffuseMap, specularMap, emissionMap, normalMap;
  float spec_exponent;
};
uniform Material material;


uniform samplerCube depthMap;
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
  // use the light to fragment vector to sample from the depth map    
  float closestDepth = texture(depthMap, fragToLight).r;
  // it is currently in linear range between [0,1]. Re-transform back to original value
  closestDepth *= far_plane;
  // now get current linear depth as the length between the fragment and light position
  float currentDepth = length(fragToLight);
  // now test for shadows
  float bias = 0.05; 
  return currentDepth - bias > closestDepth;
}

float calculate_shadow(vec3 lightPos, vec3 viewPos, vec3 fragPos, float bias)
{
  vec3 fragToLight = fragPos - lightPos;
  float currentDepth = length(fragToLight);

  float shadow = 0.0;
  int samples = 3;
  float viewDistance = length(viewPos - fragPos);
  float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
  for(int i = 0; i < samples; ++i)
  {
    float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
    closestDepth *= far_plane;   // undo mapping [0;1]
    if(currentDepth - bias > closestDepth)
      shadow += 1.0;
  }
  shadow /= float(samples);

  return shadow;
}


vec3 calculate_pointlight(PointLight light, vec3 fragPos, vec3 viewPos)
{

  // for each step, move the endpoint of the ray towards the fragment
  vec3 vol = vec3(0, 0, 0);

  float view_frag_length = length(fs_in.FragPos - fs_in.viewPos);
  int max_steps = 128;
  float min_step_size = 0.05;
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

    float light_ray_dist = length(ray - light.pos);
    float attenuation = 1.0 / (light.constant + light.linear * light_ray_dist + light.quadratic * (light_ray_dist * light_ray_dist));


    if (!in_shadow(light.pos, viewPos, ray))
      vol += 0.001 * attenuation * light.diffuse;

  }

  return (vol);
}


// vec3 calculate_spotlight(SpotLight light, vec3 fragPos, vec3 viewPos)
// {
//   vec3 lightDir = normalize(light.position - fragPos);
//   float theta = dot(lightDir, normalize(-light.direction));
//   float epsilon = light.inner_cutoff - light.outer_cutoff;
//   float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0); 
//   intensity *= light.intensity;

//   lightDir = normalize(light.position - fragPos);
//   vec3 viewDir = normalize(viewPos - fragPos);

//   // diffuse shading
//   float diff = max(dot(normal, lightDir), 0.0);

//   // specular shading
//   vec3 halfwayDir = normalize(lightDir + viewDir);  
//   float spec = pow(max(dot(normal, halfwayDir), 0.0), material.spec_exponent);

//   // attenuation
//   float distance    = length(light.position - fragPos);
//   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

//   // combine results
//   vec3 ambient  = light.ambient  * vec3(texture(material.diffuseMap, fs_in.TexCoords));
//   vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuseMap, fs_in.TexCoords));
//   vec3 specular = light.diffuse * spec * vec3(texture(material.specularMap, fs_in.TexCoords));
//   ambient  *= intensity * attenuation;
//   diffuse  *= intensity * attenuation;
//   specular *= intensity * attenuation;

//   return (ambient + diffuse + specular);
// }


void main()
{
  vec3 result = calculate_pointlight(pointlight, fs_in.FragPos, fs_in.viewPos);

  // for (int i=0; i<num_active_spotlights; i++)
  //   result += calculate_spotlight(spotlights[i], normal, fs_in.FragPos, fs_in.viewPos);

  FragColor = vec4(result, 1.0);

}