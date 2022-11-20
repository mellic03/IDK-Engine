#shader vertex

out vec4 FragColor;
uniform vec4 objectColor;
uniform vec4 lightColor;

void main()
{
  FragColor = vec4(lightColor * objectColor, 1.0);
}


#shader fragment

#version 330 core
out vec4 FragColor;
  
uniform sampler2D gSampler;
in vec3 color;

void main()
{
  float ambient_strength = 0.2;
  FragColor = vec4(color.xyz, 1);
}