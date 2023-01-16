#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 gEmission;


in vec2 TexCoords;
in vec3 FragPos;

uniform vec3 clearColor;

void main()
{
  gPosition = vec4(FragPos, 1.0);
  gNormal = vec4(0.0);
  gAlbedoSpec = vec4(clearColor, 1.0);
  gEmission = clearColor;
}
