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
uniform mat4 parent_model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * parent_model * transform  * model * vec4(aPos, 1.0);
  FragPos = vec3(model * vec4(aPos, 1.0));
  // normal = mat3(transpose(inverse(model))) * aNormal; 
  normal = aNormal; 
  TexCoord = aTexCoord;
}



#shader fragment

#version 330 core


struct Material {
  vec3 ambient;
  vec3 diffuse;
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


out vec4 FragColor;
  
uniform vec4 emmission;
uniform vec3 light_pos;
uniform vec3 view_pos;

uniform sampler2D gSampler;
in vec3 FragPos;
in vec3 normal;
in vec2 TexCoord;



void main()
{
  vec3 ambient = light.ambient * material.ambient;
  vec3 lightColor = vec3(1, 1, 1);

  vec3 lightDir = normalize(light_pos - FragPos);
  vec3 norm = normalize(normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * (diff * material.diffuse);

  float spec_strength = 0.5;
  vec3 viewDir = normalize(view_pos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.spec_exponent);
  vec3 specular = light.specular * (spec * material.specular);


  vec3 result = (ambient + diffuse + specular) * 1;

  vec4 tex = texture(gSampler, TexCoord);
  FragColor = vec4(tex.x*result.x, tex.y*result.y, tex.z*result.z, tex.w);
  // FragColor = vec4(0, TexCoord.x, TexCoord.y, 1);
}