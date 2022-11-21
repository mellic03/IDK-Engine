#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aFaceNormal;
layout (location = 3) in vec2 aTexCoord;

out vec3 normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  FragPos = vec3(model * transform * vec4(aPos, 1.0));
  normal = mat3(transpose(inverse(model * transform))) * aNormal; 
  TexCoord = aTexCoord;

  gl_Position = projection * view * vec4(FragPos, 1.0);

}





#shader fragment

#version 330 core

struct Material {
  sampler2D diffuse;
  vec3 specular;
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

uniform vec3 view_pos;

vec3 viewPos;
vec3 lightPos;

in vec3 FragPos;
in vec3 normal;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
  viewPos = vec3(vec4(view_pos, 1));
  lightPos = vec3(vec4(light.position, 1));

  vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
  
  // diffuse 
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;  
  
  // specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.spec_exponent);
  vec3 specular = light.specular * (spec * material.specular);  
      
  vec3 result = ambient + diffuse + specular;
  FragColor = vec4(result, 1.0);
}