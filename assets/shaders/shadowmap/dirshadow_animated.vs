#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

layout (location = 5) in vec4 jointWeights;
layout (location = 6) in ivec4 jointIDs;

uniform mat4 boneTransforms[64];


uniform mat4 lightSpaceMatrix;
uniform mat4 model;

out vec2 TexCoords;

void main()
{
  vec4 position = vec4(0.0);

  bool done = false;

  for (int i=0; i<4; i++)
  {
    if (jointIDs[i] == -1)
    {
      continue;
    }

    position += jointWeights[i] * boneTransforms[jointIDs[i]] * vec4(aPos, 1.0);

    done = true;
  }

  if (!done)
  {
    position = vec4(aPos, 1.0);
  }


  TexCoords = aTexCoords;
  gl_Position = lightSpaceMatrix * model * vec4(position.xyz, 1.0);
}  

