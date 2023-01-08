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
uniform PointLight pointlights[NUM_POINTLIGHTS];
uniform PointLight shadowmapped_pointlight;
uniform int num_active_pointlights;

struct SpotLight {
  vec3 position, direction;
  vec3 ambient, diffuse;
  float constant, linear, quadratic;
  float inner_cutoff, outer_cutoff, intensity;
};
uniform SpotLight spotlights[NUM_SPOTLIGHTS];
uniform SpotLight shadowmapped_spotlight;
uniform int num_active_spotlights;

in VS_OUT {

  vec4 lspacepos;

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


uniform samplerCube depthmap_pointlight;
uniform sampler2D   depthmap_dirlight;
uniform sampler2D   depthmap_spotlight;
uniform mat4 lightSpaceMatrix;

// Fog
uniform float fog_start, fog_end;
uniform vec3 clearColor;


uniform float far_plane;


vec4 hash4( vec2 p ) { return fract(sin(vec4( 1.0+dot(p,vec2(37.0,17.0)), 
                                              2.0+dot(p,vec2(11.0,47.0)),
                                              3.0+dot(p,vec2(41.0,29.0)),
                                              4.0+dot(p,vec2(23.0,31.0))))*103.0); }

vec4 textureNoTile( sampler2D samp, in vec2 uv )
{
    vec2 p = floor( uv );
    vec2 f = fract( uv );
	
    // derivatives (for correct mipmapping)
    vec2 ddx = dFdx( uv );
    vec2 ddy = dFdy( uv );
    
    // voronoi contribution
    vec4 va = vec4( 0.0 );
    float wt = 0.0;
    for( int j=-1; j<=1; j++ )
    for( int i=-1; i<=1; i++ )
    {
        vec2 g = vec2( float(i), float(j) );
        vec4 o = hash4( p + g );
        vec2 r = g - f + o.xy;
        float d = dot(r,r);
        float w = exp(-5.0*d );
        vec4 c = textureGrad( samp, uv + o.zw, ddx, ddy );
        va += w*c;
        wt += w;
    }
	
    // normalization
    return va/wt;
}


vec3 gridSamplingDisk[20] = vec3[]
(
  vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
  vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
  vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
  vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
  vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);


float calculate_shadow_pointlight(vec3 lightPos, vec3 viewPos, vec3 fragPos, float bias)
{
  vec3 fragToLight = fragPos - lightPos;
  float currentDepth = length(fragToLight);

  float shadow = 0.0;
  int samples = 20;
  float viewDistance = length(viewPos - fragPos);
  float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
  for(int i = 0; i < samples; ++i)
  {
    float closestDepth = texture(depthmap_pointlight, fragToLight + gridSamplingDisk[i] * diskRadius).r;
    closestDepth *= far_plane;   // undo mapping [0;1]
    if(currentDepth - bias > closestDepth)
      shadow += 1.0;
  }
  shadow /= float(samples);

  return shadow;
}

float calculate_shadow_dirlight(vec3 pos, vec3 normal, vec3 lightDir, float bias)
{
  vec3 projCoords = fs_in.lspacepos.xyz / fs_in.lspacepos.w;
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.z > 1.0)
    return 0.0;

  float closestDepth = texture(depthmap_dirlight, projCoords.xy).r; 
  float currentDepth = projCoords.z;
  
  float diffuseFactor = dot(normal, -lightDir);
  float bb = mix(bias, 0.0, diffuseFactor);

  float shadow = 0.0;
  vec2 texelSize = 4.0 / textureSize(depthmap_dirlight, 1);
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


float calculate_shadow_spotlight(vec3 lightPos, vec3 viewPos, vec3 fragPos, float bias)
{
  vec3 fragToLight = fragPos - lightPos;
  float currentDepth = length(fragToLight);

  float shadow = 0.0;
  int samples = 20;
  float viewDistance = length(viewPos - fragPos);
  float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
  for(int i = 0; i < samples; ++i)
  {
    float closestDepth = texture(depthmap_pointlight, fragToLight + gridSamplingDisk[i] * diskRadius).r;
    closestDepth *= far_plane;   // undo mapping [0;1]
    if(currentDepth - bias > closestDepth)
      shadow += 1.0;
  }
  shadow /= float(samples);

  return shadow;
}


vec3 calculate_dirlight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewPos)
{
  vec3 ambient = light.ambient * texture(material.diffuseMap, fs_in.TexCoords).rgb;
  
  vec3 lightDir = normalize(light.position);  
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(material.diffuseMap, fs_in.TexCoords).rgb;  

  vec3 halfwayDir = normalize(lightDir + normalize(viewPos - fragPos));  
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.spec_exponent);
  vec3 specular = spec * texture(material.specularMap, fs_in.TexCoords).rgb;
  
  float shadow = calculate_shadow_dirlight(light.position, normal, normalize(-light.position), light.bias);
  return (ambient + (1.0 - shadow) * (diffuse + specular));
}


vec3 calculate_pointlight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos)
{
  vec3 lightDir = normalize(light.position - fragPos);
  vec3 viewDir = normalize(viewPos - fragPos);

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


vec3 calculate_pointlight_shadowmapped(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos)
{
  vec3 lightDir = normalize(light.position - fragPos);
  vec3 viewDir = normalize(viewPos - fragPos);

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

  float shadow = calculate_shadow_pointlight(light.position, viewPos, fragPos, light.bias);

  return (ambient + (1.0 - shadow) * (diffuse + specular));
}


vec3 calculate_spotlight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewPos)
{
  vec3 lightDir = normalize(light.position - fragPos);
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.inner_cutoff - light.outer_cutoff;
  float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0); 
  intensity *= light.intensity;

  lightDir = normalize(light.position - fragPos);
  vec3 viewDir = normalize(viewPos - fragPos);

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
  vec3 specular = light.diffuse * spec * vec3(texture(material.specularMap, fs_in.TexCoords));
  ambient  *= intensity * attenuation;
  diffuse  *= intensity * attenuation;
  specular *= intensity * attenuation;

  return (ambient + diffuse + specular);
}

vec3 calculate_spotlight_shadowmapped(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewPos)
{
  vec3 lightDir = normalize(light.position - fragPos);
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.inner_cutoff - light.outer_cutoff;
  float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0); 
  intensity *= light.intensity;

  lightDir = normalize(light.position - fragPos);
  vec3 viewDir = normalize(viewPos - fragPos);

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
  vec3 specular = light.diffuse * spec * vec3(texture(material.specularMap, fs_in.TexCoords));
  ambient  *= intensity * attenuation;
  diffuse  *= intensity * attenuation;
  specular *= intensity * attenuation;

  float shadow = 0.0;

  return (ambient + (1.0 - shadow) * (diffuse + specular));
}


void main()
{
  vec3 normal = texture(material.normalMap, fs_in.TexCoords).rgb;
  normal = normal * 2.0 - 1.0; 
  normal = normalize(fs_in.TBN * normal);

  vec3 result = texture(material.emissionMap, fs_in.TexCoords).rgb;

  // Shadow-mapped lights
  //-------------------------
  result += calculate_pointlight_shadowmapped(shadowmapped_pointlight, normal, fs_in.FragPos, fs_in.viewPos);
  // result += calculate_spotlight_shadowmapped(shadowmapped_spotlight, normal, fs_in.FragPos, fs_in.viewPos);
  //-------------------------

  // Non shadow-mapped lights
  //-------------------------

  result += calculate_dirlight(shadowmapped_dirlight, normal, fs_in.FragPos, fs_in.viewPos);

  for (int i=0; i<num_active_pointlights; i++)
    result += calculate_pointlight(pointlights[i], normal, fs_in.FragPos, fs_in.viewPos);


  for (int i=0; i<num_active_spotlights; i++)
    result += calculate_spotlight(spotlights[i], normal, fs_in.FragPos, fs_in.viewPos);
  //-------------------------


  FragColor = vec4(result, 1.0);


  // float dist = length(fs_in.FragPos - fs_in.viewPos);
  // float fog_factor = (dist - fog_start)/(fog_end-fog_start);
  // fog_factor = 1.0 - clamp(fog_factor, 0, 1);

  // FragColor = mix(vec4(clearColor, 1.0), FragColor, fog_factor);
  // FragColor2 = FragColor;

}