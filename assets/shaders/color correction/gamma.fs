#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D inputTexture;

uniform float gamma;

void main()
{
  FragColor = pow(texture(inputTexture, TexCoords), vec4(gamma));
}
