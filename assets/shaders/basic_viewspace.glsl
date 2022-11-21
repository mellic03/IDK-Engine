#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aFaceNormal;
layout (location = 3) in vec2 aTexCoord;

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Light light;

out Light light_view;

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

  light_view = light;
  light_view.position = vec3(view * vec4(light_view.position, 1.0));

  gl_Position = projection * vec4(FragPos, 1.0);

}





#shader fragment

#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
  float spec_exponent;
};
uniform Material material;

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
in Light light_view;

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
  lightPos = vec3(vec4(light_view.position, 1));

  vec3 ambient = light_view.ambient * texture(material.diffuse, TexCoord).rgb;
  
  // diffuse 
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light_view.diffuse * diff * texture(material.diffuse, TexCoord).rgb;  
  
  // specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.spec_exponent);
  vec3 specular = light_view.specular * spec * texture(material.specular, TexCoord).rgb;

  vec3 emission = texture(material.emission, TexCoord).rgb;

  FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
}