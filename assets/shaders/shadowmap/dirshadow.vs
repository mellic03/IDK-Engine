#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

// out vec4 pos;
// out vec4 norm;
out vec2 TexCoords;

void main()
{
  TexCoords = aTexCoords;
  // pos  = model * vec4(aPos, 1.0);
  // norm = model * vec4(aNormal, 0.0);
  gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}  



