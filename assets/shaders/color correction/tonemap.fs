#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D inputTexture;

uniform float gamma;
uniform float exposure;

void main()
{
  vec3 hdrColor = texture(inputTexture, TexCoords).rgb;

  vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
  result = pow(result, vec3(1.0 / gamma));

  FragColor = vec4(result, 1.0);
}
