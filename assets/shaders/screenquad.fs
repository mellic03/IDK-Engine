#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform float kernel[9];
float kernelDivisor = 16;
uniform float kernelOffsetDivisor;

uniform sampler2D screenTexture;
uniform sampler2D volumetricLightsTexture;


uniform float gamma;
uniform float exposure;


void main()
{ 

  // Image convolution
  // -----------------------------------------
  float offset = 1.0 / kernelOffsetDivisor;
  vec2 offsets[9] = vec2[](
      vec2(-offset,  offset), // top-left
      vec2( 0.0f,    offset), // top-center
      vec2( offset,  offset), // top-right
      vec2(-offset,  0.0f),   // center-left
      vec2( 0.0f,    0.0f),   // center-center
      vec2( offset,  0.0f),   // center-right
      vec2(-offset, -offset), // bottom-left
      vec2( 0.0f,   -offset), // bottom-center
      vec2( offset, -offset)  // bottom-right    
  );

  float ImageKernel[9] = float[](
      1, // top-left
      2, // top-center
      1, // top-right
      2,   // center-left
      1,   // center-center
      2,   // center-right
      1, // bottom-left
      2, // bottom-center
      1  // bottom-right    
  );

  for (int i=0; i<9; i++)
    ImageKernel[i] = ImageKernel[i]/kernelDivisor;

  //-----------------------------------------

  vec3 sampleTex[9];
  for(int i = 0; i < 9; i++)
    sampleTex[i] = vec3(texture(volumetricLightsTexture, TexCoords.st + offsets[i]));

  vec3 RenderWithKernel = vec3(0.0);
  for(int i = 0; i < 9; i++)
    RenderWithKernel += sampleTex[i] * ImageKernel[i];
  
  vec3 hdrColor = RenderWithKernel + texture(screenTexture, TexCoords).rgb;
  vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
  result = pow(result, vec3(1.0 / gamma));

  FragColor = vec4(result, 1.0);

}
