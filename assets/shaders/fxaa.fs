#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform float bloomAmount;


// FXAA Quality Settings
#define FXAA_SPAN_MAX (8.0)
#define FXAA_REDUCE_MUL (1.0/8.0)
#define FXAA_REDUCE_MIN (1.0/128.0)

vec3 fxaa(sampler2D tex, vec2 frameBufSize)
{
  vec3 result = vec3(0.0);

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

vec3 aces(vec3 x)
{
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec3 reinhard(vec3 x)
{
  return x / (x + vec3(1.0));
}

void main() {

  vec3 color = fxaa(screenTexture, textureSize(screenTexture, 0));
  vec3 bloom = texture(bloomTexture, TexCoords).rgb;
  
  vec3 result = color;
  result = result + bloomAmount*bloom;


  result = aces(result);
  result = pow(result, vec3(1.0 / 2.2));

  FragColor = vec4(result, 1.0);
}
