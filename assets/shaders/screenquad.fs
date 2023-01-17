#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

// uniform float kernel[9];
// float kernelDivisor = 1;
// uniform float kernelOffsetDivisor;

uniform sampler2D screenTexture;
uniform sampler2D volumetricLightsTexture;
uniform sampler2D bloomTexture;


uniform float gamma;
uniform float exposure;


void main()
{
  // Image convolution
  // -----------------------------------------
  // float offset = 1.0 / kernelOffsetDivisor;
  // vec2 offsets[9] = vec2[](
  //     vec2(-offset,  offset), // top-left
  //     vec2( 0.0f,    offset), // top-center
  //     vec2( offset,  offset), // top-right
  //     vec2(-offset,  0.0f),   // center-left
  //     vec2( 0.0f,    0.0f),   // center-center
  //     vec2( offset,  0.0f),   // center-right
  //     vec2(-offset, -offset), // bottom-left
  //     vec2( 0.0f,   -offset), // bottom-center
  //     vec2( offset, -offset)  // bottom-right    
  // );

  // float ImageKernel[9] = float[](
  //     0, // top-left
  //     0, // top-center
  //     0, // top-right
  //     0,   // center-left
  //     1,   // center-center
  //     0,   // center-right
  //     0, // bottom-left
  //     0, // bottom-center
  //     0  // bottom-right    
  // );

  // for (int i=0; i<9; i++)
  //   ImageKernel[i] = ImageKernel[i]/kernelDivisor;

  // vec3 sampleTex[9];
  // for(int i = 0; i < 9; i++)
  //   sampleTex[i] = vec3(texture(volumetricLightsTexture, TexCoords.st + offsets[i]));

  // vec3 RenderWithKernel = vec3(0.0);

  // for(int i = 0; i < 9; i++)
  //   RenderWithKernel += sampleTex[i] * ImageKernel[i];

  vec3 hdrColor = vec3(0.0, 0.0, 0.0);
  hdrColor += textureLod(screenTexture, TexCoords, 0.0).rgb;

  vec3 vol = vec3(0.0);
  float levels = 4.0;
  for (float i=0; i<levels; i++)
  {
    vol += textureLod(volumetricLightsTexture, TexCoords, i).rgb;
  }
  vol /= levels;


  hdrColor += vol;
  // hdrColor += texture(bloomTexture, TexCoords).rgb;
  vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
  result = pow(result, vec3(1.0 / gamma));


  FragColor = vec4(result, 1.0);
}
