#version 330 core
layout (location = 0) out vec4 FragColor;

struct PointLight {
  vec3 pos;
  vec3 ambient, diffuse;
  float constant, linear, quadratic;
};

in VS_OUT {

  vec3 FragPos;
  vec3 viewPos;
  vec3 Normal;
  vec2 TexCoords;

  PointLight pointlight;

  mat3 TBN;

} fs_in;


struct Material {
  sampler2D diffuseMap, specularMap, emissionMap, normalMap;
  float spec_exponent;
};
uniform Material material;


uniform samplerCube depthMap;
uniform float bias;


// Fog
uniform float fog_start, fog_end;
uniform vec3 clearColor;



uniform float far_plane;


vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float calculate_shadow(vec3 lightPos, vec3 viewPos, vec3 fragPos)
{
  vec3 fragToLight = fragPos - lightPos;
  float currentDepth = length(fragToLight);

  float shadow = 0.0;
  int samples = 20;
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



vec3 calculate_pointlight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos)
{
  vec3 lightDir = normalize(light.pos - fragPos);
  vec3 viewDir = normalize(viewPos - fragPos);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // specular shading
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.spec_exponent);

  // attenuation
  float distance    = length(light.pos - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  // combine results
  vec3 ambient  = light.ambient * texture(material.diffuseMap, fs_in.TexCoords).rgb;
  vec3 diffuse  = light.diffuse * diff * texture(material.diffuseMap, fs_in.TexCoords).rgb;
  vec3 specular = light.diffuse * spec * texture(material.specularMap, fs_in.TexCoords).rgb;

  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  float shadow = calculate_shadow(light.pos, viewPos, fragPos);

  return (ambient + (1.0 - shadow) * (diffuse + specular));
}





void main()
{
  vec3 normal = texture(material.normalMap, fs_in.TexCoords).rgb;
  normal = normal * 2.0 - 1.0; 
  normal = normalize(fs_in.TBN * normal);

  vec3 result = texture(material.emissionMap, fs_in.TexCoords).rgb;

  result += calculate_pointlight(fs_in.pointlight, normal, fs_in.FragPos, fs_in.viewPos);


  FragColor = vec4(result, 1.0);


  float dist = length(fs_in.FragPos - fs_in.viewPos);
  float fog_factor = (dist - fog_start)/(fog_end-fog_start);
  fog_factor = 1.0 - clamp(fog_factor, 0, 1);
  FragColor = mix(vec4(clearColor, 1.0), FragColor, fog_factor);

}