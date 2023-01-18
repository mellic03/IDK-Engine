#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;


in vec2 TexCoords;

// uniform float kernel[9];
// float kernelDivisor = 1;
// uniform float kernelOffsetDivisor;

uniform sampler2D screenTexture;
uniform sampler2D volumetricLightsTexture;
uniform sampler2D emissionTexture;


uniform float gamma;
uniform float exposure;
uniform float bloom_threshold;


void main()
{
  vec3 hdrColor = vec3(0.0, 0.0, 0.0);
  hdrColor += texture(screenTexture, TexCoords).rgb;
  hdrColor += texture(volumetricLightsTexture, TexCoords).rgb;


  float brightness = dot(hdrColor.rgb, vec3(0.2126, 0.7152, 0.0722));
  if(brightness > bloom_threshold)
  {
    BrightColor = vec4(hdrColor, 1.0);
  }
  else
  {
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
  }


  // vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
  // result = pow(result, vec3(1.0 / gamma));

  FragColor = vec4(hdrColor, 1.0);
}
