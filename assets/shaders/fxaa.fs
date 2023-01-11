#version 330 core

uniform sampler2D tex;
uniform vec2 resolution;

// FXAA Quality Settings
uniform float FXAA_SPAN_MAX = 8.0;
uniform float FXAA_REDUCE_MUL = 1.0/8.0;
uniform float FXAA_SUBPIX_SHIFT = 1.0/4.0;

vec4 fxaa(sampler2D tex, vec2 pos, vec2 rcpFrame)
{
  vec4 rgbNW = texture(tex, (pos + vec2(-1.0, -1.0)) * rcpFrame);
  vec4 rgbNE = texture(tex, (pos + vec2( 1.0, -1.0)) * rcpFrame);
  vec4 rgbSW = texture(tex, (pos + vec2(-1.0,  1.0)) * rcpFrame);
  vec4 rgbSE = texture(tex, (pos + vec2( 1.0,  1.0)) * rcpFrame);
  vec4 rgbM  = texture(tex,  pos  * rcpFrame);
  vec4 luma = vec4(0.299, 0.587, 0.114, 1.0);
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
  float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_SPAN_MAX);
  float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
  dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
        max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
        dir * rcpDirMin)) * rcpFrame.xy;
  vec4 rgbA = (1.0/2.0) * (
      texture(tex, pos.xy + dir * (1.0/3.0 - 0.5)) +
      texture(tex, pos.xy + dir * (2.0/3.0 - 0.5)));
   
  vec4 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
      texture(tex, pos.xy + dir * (0.0/3.0 - 0.5)) +
      texture(tex, pos.xy + dir * (3.0/3.0 - 0.5)));
  float lumaB = dot(rgbB, vec4(0.299, 0.587, 0.114, 0.0));

  if ((lumaB < lumaMin) || (lumaB > lumaMax))
      return vec4(rgbA.rgb, 1.0);
  else
      return vec4(rgbB.rgb, 1.0);
}

void main() {
    vec2 rcpFrame = vec2(1.0/resolution.x, 1.0/resolution.y);
    vec2 pos = gl_FragCoord.xy * rcpFrame;
    gl_FragColor = fxaa(tex, pos, rcpFrame);
}
