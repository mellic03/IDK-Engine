#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aFaceNormal;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;


out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
  vs_out.FragPos = vec3(model * transform * vec4(aPos, 1.0));
  vs_out.TexCoords = aTexCoords;

  mat3 normalMatrix = transpose(inverse(mat3(model * transform)));
  vec3 T = normalize(normalMatrix * aTangent);
  vec3 N = normalize(normalMatrix * aNormal);
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);
  
  mat3 TBN = transpose(mat3(T, B, N));    
  vs_out.TangentLightPos = TBN * vec3(view * vec4(lightPos, 1.0));
  vs_out.TangentViewPos  = TBN * viewPos;
  vs_out.TangentFragPos  = TBN * vs_out.FragPos;


  gl_Position = projection * model * transform * vec4(aPos, 1.0);

}





#shader fragment

#version 330 core

struct Material {
  sampler2D diffuseMap;
  sampler2D specularMap;
  sampler2D emissionMap;
  sampler2D normalMap;
  float spec_exponent;
};
uniform Material material;

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Light light;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;


out vec4 FragColor;


void main()
{
  vec3 normal = texture(material.normalMap, fs_in.TexCoords).rgb;
  // transform normal vector to range [-1,1]
  normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

  vec3 lightDir = normalize(fs_in.TangentLightPos-fs_in.TangentFragPos);
  vec3 viewDir = normalize(-fs_in.TangentFragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);  

  vec3 color = texture(material.diffuseMap, fs_in.TexCoords).rgb;
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * light.diffuse * color;


  vec3 ambient = light.ambient * color;


  float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
  vec3 specular = light.specular * spec;


  vec3 emission = texture(material.emissionMap, fs_in.TexCoords).rgb;

  FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
}