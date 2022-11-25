#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform float kernel[9];
uniform float kernelDivisor;
uniform float kernelOffsetDivisor;
uniform sampler2D screenTexture;

uniform float gamma;
uniform float exposure;

void main()
{ 

  // Image convolution
  //-----------------------------------------
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

  float ImageKernel[9];

  for (int i=0; i<9; i++)
    ImageKernel[i] = kernel[i]/kernelDivisor;

  //-----------------------------------------

  vec3 sampleTex[9];
  for(int i = 0; i < 9; i++)
    sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
  vec3 RenderWithKernel = vec3(0.0);
  for(int i = 0; i < 9; i++)
    RenderWithKernel += sampleTex[i] * ImageKernel[i];
  

  vec3 hdrColor = RenderWithKernel;

  vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
  mapped = pow(mapped, vec3(1.0 / gamma));

  FragColor = vec4(mapped, 1.0);
}