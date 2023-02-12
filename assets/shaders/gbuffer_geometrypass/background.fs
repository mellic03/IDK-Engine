#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gEmission;


in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 skyColor;
uniform vec3 horizonColor;
uniform float start;
uniform float stop;

uniform vec3 ViewPos;
uniform vec3 SunPos;
vec3 SunColor = vec3(1.0, 1.0, 1.0);


void main()
{
  gPosition = vec4(FragPos, 1.0);
  gNormal = vec4(0.0);


  vec3 result = vec3(0.0);
  result += skyColor;

  vec3 dir = normalize(gPosition.xyz - ViewPos);
  
  vec3 sun_pos = normalize(1000 * SunPos);
  float sun_strength = dot(dir, sun_pos);
  if (sun_strength > 0.9995)
  {
    sun_strength = (sun_strength + 1.0) / 2.0;
    result += exp(1*sun_strength) * SunColor;
  }


  float horizonStrength = (1.0 - abs(dir.y + 0.1));

  if (dir.y < -0.1)
  {
    result = mix(result, horizonColor, 1);
  }

  else
  {
    result = mix(result, horizonColor, smoothstep(start, stop, horizonStrength));
  }

  gEmission.rgb += result;
  gAlbedoSpec.rgb += result;
  gAlbedoSpec.a = 0.0;

}
