#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gEmission;


void main()
{
  gPosition = vec4(0.0);
  gNormal = vec4(0.0);
  gAlbedoSpec = vec4(1.0);
  gEmission = vec4(1.0);
}
