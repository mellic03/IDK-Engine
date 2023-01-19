#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gEmission;


in vec2 TexCoords;
in vec3 FragPos;

uniform vec3 clearColor;

uniform vec3 ViewPos;
uniform vec3 SunPos;
vec3 SunColor = vec3(1.0, 0.6, 0.2);


void main()
{
  gPosition = vec4(FragPos, 1.0);
  gNormal = vec4(0.0);
  // gAlbedoSpec = vec4(clearColor, 0.0);
  gEmission = vec4(clearColor, 0.0);


  vec3 dir = normalize(gPosition.xyz - ViewPos);

  float strength = dot(dir, vec3(0.0, 1.0, 0.0));
  strength = (strength + 1.0) / 2.0;
  gEmission += strength * vec4(clearColor, 0.0);


  vec3 sun_pos = normalize(1000 * SunPos);
  float sun_strength = dot(dir, sun_pos);
  if (sun_strength > 0.998)
  {
    sun_strength = (sun_strength + 1.0) / 2.0;
    gEmission += exp(4*sun_strength) * vec4(SunColor, 1.0);
  }

}
