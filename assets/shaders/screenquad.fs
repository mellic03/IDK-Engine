#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform float kernel[9];
float kernelDivisor = 1;
uniform float kernelOffsetDivisor;

uniform sampler2D screenTexture;
uniform sampler2D volumetricLightsTexture;
uniform sampler2D bloomTexture;


uniform float gamma;
uniform float exposure;

vec3 fxaa(sampler2D tex, vec2 frameBufSize)
{
  vec3 result = vec3(0.0);

  float FXAA_SPAN_MAX = 8.0;
  float FXAA_REDUCE_MUL = 1.0/8.0;
  float FXAA_REDUCE_MIN = 1.0/128.0;

  vec3 rgbNW=texture2D(tex, TexCoords+(vec2(-1.0,-1.0)/frameBufSize)).xyz;
  vec3 rgbNE=texture2D(tex, TexCoords+(vec2(1.0,-1.0)/frameBufSize)).xyz;
  vec3 rgbSW=texture2D(tex, TexCoords+(vec2(-1.0,1.0)/frameBufSize)).xyz;
  vec3 rgbSE=texture2D(tex, TexCoords+(vec2(1.0,1.0)/frameBufSize)).xyz;
  vec3 rgbM=texture2D(tex, TexCoords).xyz;

  vec3 luma=vec3(0.299, 0.587, 0.114);
  float lumaNW = dot(rgbNW, luma);
  float lumaNE = dot(rgbNE, luma);
  float lumaSW = dot(rgbSW, luma);
  float lumaSE = dot(rgbSE, luma);
  float lumaM  = dot(rgbM,  luma);

  float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
  float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

  vec2 dir;
  dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
  dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

  float dirReduce = max(
      (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
      FXAA_REDUCE_MIN);

  float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);

  dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
        max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
        dir * rcpDirMin)) / frameBufSize;

  vec3 rgbA = (1.0/2.0) * (
      texture2D(tex,  TexCoords.xy + dir * (1.0/3.0 - 0.5)).xyz +
      texture2D(tex,  TexCoords.xy + dir * (2.0/3.0 - 0.5)).xyz);
  vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
      texture2D(tex,  TexCoords.xy + dir * (0.0/3.0 - 0.5)).xyz +
      texture2D(tex,  TexCoords.xy + dir * (3.0/3.0 - 0.5)).xyz);
  float lumaB = dot(rgbB, luma.xyz);

  if((lumaB < lumaMin) || (lumaB > lumaMax)){
      result.xyz=rgbA;
  }else{
      result.xyz=rgbB;
  }

  return result;
}


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
      0, // top-left
      0, // top-center
      0, // top-right
      0,   // center-left
      1,   // center-center
      0,   // center-right
      0, // bottom-left
      0, // bottom-center
      0  // bottom-right    
  );

  // for (int i=0; i<9; i++)
  //   ImageKernel[i] = ImageKernel[i]/kernelDivisor;

  // vec3 sampleTex[9];
  // for(int i = 0; i < 9; i++)
  //   sampleTex[i] = vec3(texture(volumetricLightsTexture, TexCoords.st + offsets[i]));

  // vec3 RenderWithKernel = vec3(0.0);

  // for(int i = 0; i < 9; i++)
  //   RenderWithKernel += sampleTex[i] * ImageKernel[i];

  vec3 hdrColor = vec3(0.0, 0.0, 0.0);
  hdrColor += fxaa(screenTexture, textureSize(screenTexture, 0));
  // hdrColor += texture(screenTexture, TexCoords).rgb;
  // hdrColor += fxaa(volumetricLightsTexture, textureSize(screenTexture, 0));
  hdrColor += texture(volumetricLightsTexture, TexCoords).rgb;

  vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
  result = pow(result, vec3(1.0 / gamma));


  FragColor = vec4(result, 1.0);
}
