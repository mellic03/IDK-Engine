#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;


uniform vec3 diffuse;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragColor;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  fragColor = diffuse;
}


#shader fragment

#version 330 core

in vec3 fragColor;
out vec4 FragColor;
  
void main()
{
  FragColor = vec4(fragColor.xyz, 1);
}