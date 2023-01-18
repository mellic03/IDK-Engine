#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D inputTexture;

uniform float gamma;
uniform float exposure;

vec3 filmic(vec3 x) {
  vec3 X = max(vec3(0.0), x - 0.004);
  vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
  return pow(result, vec3(2.2));
}

void main()
{
  vec3 result = texture(inputTexture, TexCoords).rgb;
  
  result = filmic(result);

  FragColor = vec4(result, 1.0);
}
