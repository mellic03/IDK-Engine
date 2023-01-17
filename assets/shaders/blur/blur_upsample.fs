#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D inputTexture1;
uniform sampler2D inputTexture2;
uniform sampler2D inputTexture3;
uniform sampler2D inputTexture4;
uniform sampler2D inputTexture5;
uniform sampler2D inputTexture6;
uniform sampler2D inputTexture7;
uniform sampler2D inputTexture8;


void main()
{
  vec3 result = vec3(0.0);

  result += texture(inputTexture1, TexCoords).rgb;
  result += texture(inputTexture2, TexCoords).rgb;
  result += texture(inputTexture3, TexCoords).rgb;
  result += texture(inputTexture4, TexCoords).rgb;
  result += texture(inputTexture5, TexCoords).rgb;
  result += texture(inputTexture6, TexCoords).rgb;
  result += texture(inputTexture7, TexCoords).rgb;
  result += texture(inputTexture8, TexCoords).rgb;

  FragColor = vec4(result, 1.0);
}
