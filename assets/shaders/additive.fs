#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture_1;
uniform sampler2D texture_2;
  
void main()
{             
  FragColor = texture(texture_1, TexCoords) + texture(texture_2, TexCoords);
}
