#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 parent_model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * parent_model * transform  * model * vec4(aPos, 1.0);
  TexCoord = aTexCoord;
}



#shader fragment

#version 330 core
out vec4 FragColor;
  
uniform sampler2D gSampler;
in vec3 Normal;
in vec2 TexCoord;

void main()
{
  FragColor = texture(gSampler, TexCoord);
  // FragColor = vec4(0, TexCoord.x, TexCoord.y, 1);
}